#include "server.h"

/* For Visual Studio Compilers */
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
/*----------------------------*/

void server::start_session() {
    initialise_wsa();
    create_listener();
    activate_listener();
    kick_threads();
}

void server::initialise_wsa() {
    rchat::printstart("WSA is starting up...");
    
    // Initialise winsock2 
    _result = WSAStartup(MAKEWORD(2,2), &_wsa_data);
    if(_result != 0) {
        printf("WSAStartup failed with error: %d\n", _result);
        return;
    }

    rchat::linebreak();

    ZeroMemory(&_hints, sizeof(_hints));
    _hints.ai_family = AF_INET;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_protocol = IPPROTO_TCP;
    _hints.ai_flags = AI_PASSIVE;
}

void server::create_listener() { 
    rchat::printstart("Creating listener...");

    // Translation from host name to an address getting all sockets that could be used for connection
    _result = getaddrinfo(NULL, _port, &_hints, &_addr_results);
    if(_result != 0 ) {
        printf("getaddrinfo failed with error: %d\n", _result);
        WSACleanup();
        return; 
    }
    
    // Create a socket
    _listener = socket(_addr_results->ai_family, _addr_results->ai_socktype, _addr_results->ai_protocol);
    if(_result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(_addr_results);
        closesocket(_listener);
        WSACleanup();
        return; 
    }

    // Binds the local address to a socket - it is ready to listen
    _result = bind(_listener, _addr_results->ai_addr, (int)_addr_results->ai_addrlen);
    if(_result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(_addr_results);
        closesocket(_listener);
        WSACleanup();
    }

    freeaddrinfo(_addr_results);
    
    rchat::linebreak();
}

void server::activate_listener() {
    rchat::printstart("Server listening...");

    // Set the socket to listen - waiting for a connection from a client
    _result = listen(_listener, SOMAXCONN);
    if(_result == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(_listener);
        WSACleanup();
        return;
    }
    rchat::linebreak();
}

void server::kick_threads() {
    std::thread sender(&server::send_handler, this);
    std::thread accept_connection(&server::accept_connection, this);
    accept_connection.join();
    sender.join();
    for(client_socket_info& client : _clients) {
        client._receive_ref.join();
    }
}

void server::accept_connection() {
    while(true) {
        // Accepts the connection from the first socket on the queue and assigns it
        SOCKET client_socket = accept(_listener, NULL, NULL);
        _ready_to_send = true;
        rchat::printstart("Attemping to accept connection...");
        if(client_socket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(_listener);
            WSACleanup();
            return; 
        }
        _clients.push_back(client_socket_info(client_socket, 0));
        rchat::linebreak();
    }
}

void server::send_handler() {
    int result; 
     while(true) {
         // Handle input 
        if(_ready_to_send){
            char sendbuf[64];
            rchat::printsession("Send message: ");
            scanf("%s", sendbuf);
            for(client_socket_info& client : _clients){
                SOCKET current_client = client._client;
                result = send(current_client, sendbuf, (int)strlen(sendbuf), 0);
                if(result == SOCKET_ERROR) {
                    rchat::printerrorld("Send failed with error", WSAGetLastError());
                    closesocket(current_client);
                    WSACleanup();
                    return;
                }
            }
        }
    }
}

void server::client_socket_info::receive_handler() {
    char recvbuf[512];
    int result;
    int recvbuflen = 512;
    while(true) {
        result = recv(_client, recvbuf, recvbuflen, 0 );
        if(result > 0 ) {
            if(result == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(_client);
                WSACleanup();
                return;
            }
            printf("\nmessage received: %s", recvbuf);
        }
        else if(result == 0) {
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(_client);
            WSACleanup();
            return;
        }  
    }     
}



