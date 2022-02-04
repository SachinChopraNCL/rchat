#define _WIN32_WINNT 0x501

#include <winsock2.h> 
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h> 
#include <thread>
#include "formatting.h"


#define BUFLEN 512
#define PORT "27015"

bool readyToSend = false; 

void SendHandler(SOCKET client){
    int result; 
     while(true) {
         // handle input 
        if(readyToSend){
            char sendbuf[64];
            rchat::printsession("Send message: ");
            scanf("%s", sendbuf);
            result = send(client , sendbuf, (int)strlen(sendbuf), 0);
            if(result == SOCKET_ERROR) {
                rchat::printerrorld("Send failed with error", WSAGetLastError());
                closesocket(client);
                WSACleanup();
                return ;
            }
        }
     }
}

void ReceiveHandler(SOCKET client){
    char recvbuf[BUFLEN];
    int result;
    int recvbuflen = BUFLEN;
    while(true) {
        result = recv(client, recvbuf, recvbuflen, 0 );
        readyToSend = true;
        if(result > 0 ) {
            readyToSend = true;
            if(result == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(client);
                WSACleanup();
                return;
            }
            printf("\nmessage received: %s", recvbuf);
        }
        else if(result == 0) {
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(client);
            WSACleanup();
            return;
        }  
    } 

}


int __cdecl main(void) {
    printf("--------------------------------\n");
    printf("--------------------------------\n");
    printf("server interface\n");

    WSADATA wsaData;

    SOCKET listener = INVALID_SOCKET; 
    SOCKET client = INVALID_SOCKET;

    struct addrinfo* addrResults = NULL;
    struct addrinfo hints; 
    
    int result; 
    int sendResult; 
    
    char recvbuf[BUFLEN];
    int recvbuflen = BUFLEN;

    // Initialise winsock2 
    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Translation from host name to an address getting all sockets that could be used for connection
    result = getaddrinfo(NULL, PORT, &hints, &addrResults);
    if(result != 0 ) {
        printf("getaddrinfo failed with error: %d\n", result);
        WSACleanup();
        return 1; 
    }
    
    // Create a socket
    listener = socket(addrResults->ai_family, addrResults->ai_socktype, addrResults->ai_protocol);
    if(result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(addrResults);
        closesocket(listener);
        WSACleanup();
        return 1; 
    }

    // Binds the local address to a socket - it is ready to listen
    result = bind(listener, addrResults->ai_addr, (int)addrResults->ai_addrlen);
    if(result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(addrResults);
        closesocket(listener);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(addrResults);

    // Set the socket to listen - waiting for a connection from a client
    result = listen(listener, SOMAXCONN);
    if(result == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listener);
        WSACleanup();
        return 1;
    }

    // Accepts the connection from the first socket on the queue and assigns it
    client = accept(listener, NULL, NULL);
    if(client == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(listener);
        WSACleanup();
        return 1; 
    }
    
    std::thread sender(SendHandler, client);
    std::thread receiver(ReceiveHandler, client);

      
    sender.join();
    receiver.join();

    result = shutdown(client, SD_SEND);
    if(result == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(client);
        WSACleanup();
        return 1;
    }

    // Disabling sends or receives on these sockets
    closesocket(listener);
    closesocket(client);
    
    WSACleanup();

    return 0; 
}