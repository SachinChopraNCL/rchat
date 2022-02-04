#include "client.h"

/* For Visual Studio Compilers */
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
/*----------------------------*/

void Client::StartSession() {
    InitialiseWSA();
    FindServerAndConnect();
    KickThreads(); 
}


void Client::InitialiseWSA() {
    rchat::printstart("WSA is starting up...");
    
    // Initialise winsock2 
    _result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(_result != 0) { 
        rchat::printerrori("WSAStartup failed with error", _result);
    }

    rchat::linebreak();

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
}

void Client::FindServerAndConnect(){
    rchat::printstart("Getting server information...");

    // Translation from host name to an address getting all sockets that could be used for connection
    _result = getaddrinfo(_ip, _port, &hints, &addrResults);
    if(_result != 0) {
        rchat::printerrori("Getaddrinfo failed with error", _result);
        WSACleanup();
        return;
    }

    // Check which server socket is valid and available
    for(ptrToAddr = addrResults; ptrToAddr != NULL;  ptrToAddr = ptrToAddr->ai_next) {
        server  = socket(ptrToAddr->ai_family, ptrToAddr->ai_socktype, ptrToAddr->ai_protocol);
        if(server  == INVALID_SOCKET) {
            rchat::printerrorld("Socket failed with error", WSAGetLastError());
            WSACleanup();
            return; 
        }

        // Attempt connection to the socket
        _result = connect(server , ptrToAddr->ai_addr, (int)ptrToAddr->ai_addrlen);
        if(_result == SOCKET_ERROR) {
            closesocket(server );
            server  = INVALID_SOCKET; 
            continue;
        }
        printf("Connected!");
        break;
    }
    
    freeaddrinfo(addrResults);

    if(server  == INVALID_SOCKET) {
        rchat::printerror("Unable to connect to server!");
        WSACleanup();
        return; 
    }

    rchat::linebreak();
}

void Client::KickThreads() {
    char* ackSendBuf = "ACK";
    _result = send(server , ackSendBuf, (int)strlen(ackSendBuf), 0);
    if(_result == SOCKET_ERROR) {
        rchat::printerrorld("Send failed with error", WSAGetLastError());
        closesocket(server);
        WSACleanup();
        return; 
    }

    std::thread sender(&Client::SendHandler, this);
    std::thread receiver(&Client::ReceiveHandler, this);
   
    sender.join();
    receiver.join();
}

void Client::SendHandler() {
    int result; 
    while(true) {
        // handle input 
        char sendbuf[_buflen];
        rchat::printsession("Send message: ");
        scanf("%s", sendbuf);
        result = send(server , sendbuf, (int)strlen(sendbuf), 0);
        if(result == SOCKET_ERROR) {
            rchat::printerrorld("Send failed with error", WSAGetLastError());
            closesocket(server);
            WSACleanup();
            return;
        }
    }
}

void Client::ReceiveHandler() {
    char recvbuf[_buflen];
    int result;
    while(true) {
        result = recv(server , recvbuf, _buflen, 0);
        if(result > 0) { 
            rchat::fprintsession("Server Response", recvbuf); 
        }   
        else if(result < 0){  
            rchat::printerrorld("Recv failed with error", WSAGetLastError()); 
        }
    }   
}


void Client::EndSession() {
    _result = shutdown(server , SD_SEND);
    if(_result == SOCKET_ERROR) {
        rchat::printerrorld("Shutdown failed with error", WSAGetLastError());
        closesocket(server);
        server  = INVALID_SOCKET;
        return;
    } 

    closesocket(server);
    WSACleanup();
}



