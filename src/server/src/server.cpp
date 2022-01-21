#define _WIN32_WINNT 0x501

#include <winsock2.h> 
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h> 

#define BUFLEN 512
#define PORT "27015"

int __cdecl main(void) {
    printf("SERVER INTERFACE\n");

    WSADATA wsaData;
    int iResult; 

    SOCKET listenSocket = INVALID_SOCKET; 
    SOCKET clientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints; 

    int iSendResult; 
    char recvbuf[BUFLEN];
    int recvbuflen = BUFLEN;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, PORT, &hints, &result);
    if(iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1; 
    }

    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1; 
    }

    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if(iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(listenSocket, SOMAXCONN);
    if(iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    clientSocket = accept(listenSocket, NULL, NULL);
    if(clientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1; 
    }

    closesocket(listenSocket);

    do {
        iResult = recv(clientSocket, recvbuf, recvbuflen, 0 );
        if(iResult > 0 ) {
            printf("bytes Receieved: %d\n", iResult);

            iSendResult = send(clientSocket, recvbuf, iResult, 0);
            if(iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            printf("bytes sent: %d\n", iSendResult);
        }
        else if(iResult == 0) {
            printf("connection closing...\n");
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    } while(iResult > 0);

    iResult = shutdown(clientSocket, SD_SEND);
    if(iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0; 
}