#include "client.h"

/* For Visual Studio Compilers */
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
/*----------------------------*/

void client::start_session() {
    initialise_wsa();
    find_server_connect();
    kick_threads(); 
}


void client::initialise_wsa() {
    rchat::printstart("WSA is starting up...");
    
    // Initialise winsock2 
    _result = WSAStartup(MAKEWORD(2,2), &_wsa_data);
    if(_result != 0) { 
        rchat::printerrori("WSAStartup failed with error", _result);
    }

    rchat::linebreak();

    ZeroMemory(&_hints, sizeof(_hints));
    _hints.ai_family = AF_UNSPEC;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_protocol = IPPROTO_TCP;
}

void client::find_server_connect(){
    rchat::printstart("Getting server information...");

    // Translation from host name to an address getting all sockets that could be used for connection
    _result = getaddrinfo("localhost", _port, &_hints, &_addr_results);
    if(_result != 0) {
        rchat::printerrori("Getaddrinfo failed with error", _result);
        WSACleanup();
        return;
    }

    // Check which server socket is valid and available
    for(_ptr_to_addr = _addr_results; _ptr_to_addr != NULL;  _ptr_to_addr = _ptr_to_addr->ai_next) {
        _server  = socket(_ptr_to_addr->ai_family, _ptr_to_addr->ai_socktype, _ptr_to_addr->ai_protocol);
        if(_server  == INVALID_SOCKET) {
            rchat::printerrorld("Socket failed with error", WSAGetLastError());
            WSACleanup();
            return; 
        }

        // Attempt connection to the socket
        _result = connect(_server , _ptr_to_addr->ai_addr, (int)_ptr_to_addr->ai_addrlen);
        if(_result == SOCKET_ERROR) {
            closesocket(_server );
            _server  = INVALID_SOCKET; 
            continue;
        }
        printf("Connected!");
        break;
    }
    
    freeaddrinfo(_addr_results);

    if(_server  == INVALID_SOCKET) {
        rchat::printerror("Unable to connect to server!");
        WSACleanup();
        return; 
    }

    rchat::linebreak();
}

void client::kick_threads() {
    char* ackSendBuf = "ACK";
    _result = send(_server , ackSendBuf, (int)strlen(ackSendBuf), 0);
    if(_result == SOCKET_ERROR) {
        rchat::printerrorld("Send failed with error", WSAGetLastError());
        closesocket(_server);
        WSACleanup();
        return; 
    }

    std::thread sender(&client::send_handler, this);
    std::thread receiver(&client::receive_handler, this);
   
    sender.join();
    receiver.join();
}

void client::send_handler() {
    int result; 
    while(true) {
        // handle input 
        char sendbuf[rchat::BUF_LEN];
        rchat::printsession("Send message: ");
        scanf("%s", sendbuf);
        result = send(_server , sendbuf, (int)strlen(sendbuf), 0);
        if(result == SOCKET_ERROR) {
            rchat::printerrorld("Send failed with error", WSAGetLastError());
            closesocket(_server);
            WSACleanup();
            return;
        }
    }
}

void client::receive_handler() {
    char recvbuf[rchat::BUF_LEN];
    int result;
    while(true) {
        result = recv(_server , recvbuf, rchat::BUF_LEN, 0);
        if(result > 0) { 
            rchat::fprintsession("Server Response", recvbuf); 
        }   
        else if(result < 0){  
            rchat::printerrorld("Recv failed with error", WSAGetLastError()); 
        }
    }   
}


void client::end_session() {
    _result = shutdown(_server , SD_SEND);
    if(_result == SOCKET_ERROR) {
        rchat::printerrorld("Shutdown failed with error", WSAGetLastError());
        closesocket(_server);
        _server  = INVALID_SOCKET;
        return;
    } 

    closesocket(_server);
    WSACleanup();
}



