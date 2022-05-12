#include <server.h>
#include <logging.h>

/* For Visual Studio Compilers */
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
/*----------------------------*/

using namespace rchat; 

// Global lock for the client list
std::mutex _g_client_list_mutex; 

void server::start_session() {
    line();
    log(message_type::START,"Server Interface");
    initialise_wsa();
    create_listener();
    activate_listener();
    kick_threads();
}

void server::initialise_wsa() {
    line(); 
    log(message_type::START, "WSA is starting up...");
    
    // Initialise winsock2 
    _result = WSAStartup(MAKEWORD(2,2), &_wsa_data);
    if(_result != 0) {
        printf("WSAStartup failed with error: %d\n", _result);
        return;
    }


    ZeroMemory(&_hints, sizeof(_hints));
    _hints.ai_family = AF_INET;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_protocol = IPPROTO_TCP;
    _hints.ai_flags = AI_PASSIVE;
}

void server::create_listener() { 
    line(); 
    log(message_type::START, "Creating listener...");

    // Translation from host name to an address getting all sockets that could be used for connection
    _result = getaddrinfo(NULL, _port, &_hints, &_addr_results);
    if(_result != 0 ) {
        log(message_type::ERR, "getaddrinfo failed with error: ", _result);
        WSACleanup();
        return; 
    }
    
    // Create a socket
    _listener = socket(_addr_results->ai_family, _addr_results->ai_socktype, _addr_results->ai_protocol);
    if(_result == SOCKET_ERROR) {
        log(message_type::ERR, "bind failed with error: ", WSAGetLastError());
        freeaddrinfo(_addr_results);
        closesocket(_listener);
        WSACleanup();
        return; 
    }

    // Binds the local address to a socket - it is ready to listen
    _result = bind(_listener, _addr_results->ai_addr, (int)_addr_results->ai_addrlen);
    if(_result == SOCKET_ERROR) {
        log(message_type::ERR, "bind failed with error: ", WSAGetLastError());
        freeaddrinfo(_addr_results);
        closesocket(_listener);
        WSACleanup();
    }

    freeaddrinfo(_addr_results);
    
}

void server::activate_listener() {
    line(); 
    log(message_type::START, "Server listening...");

    // Set the socket to listen - waiting for a connection from a client
    _result = listen(_listener, SOMAXCONN);
    if(_result == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(_listener);
        WSACleanup();
        return;
    }

}

void server::kick_threads() {
    std::thread accept_connections(&server::accept_connection, this);
    std::thread broadcaster(&server::broadcast_handler, this);

    accept_connections.join();
    broadcaster.join(); 
    for(client_socket_info* client : _clients) {
        client->_receive_ref.join();
    }
}

void server::accept_connection() {
    while(true) {
        // Accepts the connection from the first socket on the queue and assigns it
        SOCKET client_socket = accept(_listener, NULL, NULL);
        line(); 
        _ready_to_send = true;
        log(message_type::START, "New client is connected to server...");
        line();
        if(client_socket == INVALID_SOCKET) {
            log(message_type::ERR, "Accept failed with error: ", WSAGetLastError());
            closesocket(_listener);
            WSACleanup();
            return; 
        }
                
        bool found_existing_client = false;

        for(client_socket_info* client: _clients) { 
                if(client->_id == _client_id + 1) {
                    found_existing_client = true;
                    continue; 
                }
        }

        if(!found_existing_client) {
            std::lock_guard<std::mutex> guard(_g_client_list_mutex);
            _clients.push_back(new client_socket_info(client_socket, _client_id));
        }

        _client_id++; 
    }
}

void server::prune_clients(){ 
    int result; 
    // delete any disabled clients
    for(client_socket_info* client : _clients) {
        if(!client->_is_active) {
            line();
            log(message_type::SESSION, "Client is disconnecting with ID...", client->_id);
            line();
            _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
            client->_receive_ref.join();    
            delete client; 
        }
    }
}

void server::broadcast_handler(){
    int result; 
    while(true) {
        // Broadcasting to other clients
        if(_ready_to_send) {
            // Lock the list
            std::lock_guard<std::mutex> guard(_g_client_list_mutex);
            prune_clients(); 
            for(client_socket_info* client : _clients) {
                if(client->_message_queue.empty()) continue;
                // Lock the message queue
                std::lock_guard<std::mutex> guard(client->_m_message_queue_mutex);
                rchat::message msg_to_send = client->_message_queue.front();
                for(client_socket_info* other_client: _clients) {
                    if(client->_id == other_client->_id) continue; 
                    char sending_message_buf[global_network_variables::buflen];
                    snprintf(sending_message_buf, sizeof sending_message_buf, "Client %i: %s", client->_id, msg_to_send._content);
                    result = send(other_client->_client_socket, sending_message_buf, (int)strlen(sending_message_buf) + 1,0);
                    if(result == SOCKET_ERROR) {
                        log(message_type::ERR, "Send failed with error", WSAGetLastError());
                        closesocket(client->_client_socket);
                        WSACleanup();
                        return;
                    }
                }
                log(message_type::SESSION, "Client ", client->_id, ": ",msg_to_send._content);
                client->_message_queue.pop();
            }
        }
    }
}

void client_socket_info::receive_handler() {
    int result;
    while(true) {
        result = recv(_client_socket, _msg._content, rchat::global_network_variables::buflen, 0 );
        if(result > 0 ) {

            bool exit = rchat::global_network_variables::exit.compare(_msg._content) == 0 || result == SOCKET_ERROR;

            if(exit) {
                _is_active = false; 
                closesocket(_client_socket);
                if(result == SOCKET_ERROR) {
                    log(message_type::ERR, "send failed with error: ", WSAGetLastError());
                }
                return;
            }
            
            // Lock the message queue
            std::lock_guard<std::mutex> guard(_m_message_queue_mutex);
            _message_queue.push(_msg);
        }
        else if(result == 0) {
        }
        else {
            log(message_type::ERR, "recv failed with error: ", WSAGetLastError());
            closesocket(_client_socket);
            WSACleanup();
            return;
        }  
    }     
}





