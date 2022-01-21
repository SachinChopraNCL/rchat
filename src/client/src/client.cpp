/* For mingw compiler */
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream> 

/* For Visual Studio Compilers */
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
/*----------------------------*/

#define BUFLEN 512
#define PORT "27015"

void linebreak()
{
    printf("\n--------------------------------\n");
}

int __cdecl main(int argc, char** argv){
    
    printf("CLIENT INTERFACE");
    linebreak();

    WSADATA wsaData; 
    SOCKET connectSocket = INVALID_SOCKET; 
    struct addrinfo  *result = NULL, 
                     *ptr = NULL,
                     hints;

    char recvbuf[BUFLEN];
    int iResult;
    int recvbuflen = BUFLEN;

    if(argc != 2) {
        printf("\nusages: %s server-name\n", argv[0]);
        return 1;
    }

    printf("WSA is starting up...");
    
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0) { 
        printf("WSAStartup failed with error: %d", iResult);
    }

    linebreak();

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    printf("getting server information...");

    iResult = getaddrinfo(argv[1], PORT, &hints, &result);
    if(iResult != 0) {
        printf("getaddrinfo failed with error: %d", iResult);
        WSACleanup();
        return 1;
    }

    for(ptr = result; ptr != NULL;  ptr = ptr->ai_next) {
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(connectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld", WSAGetLastError());
            WSACleanup();
            return 1; 
        }

        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if(iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET; 
            continue;
        }
        printf("connected!");
        break;
    }
    
    linebreak();
    freeaddrinfo(result);

    if(connectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1; 
    }

    printf("connection session is progress - id 0\n");

    std::string sendbuf;
    printf("message: ");
    std::cin >> sendbuf;

    iResult = send(connectSocket, sendbuf.c_str(), (int)strlen(sendbuf.c_str()), 0);
    if(iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }
    
    do {
        iResult = recv(connectSocket, recvbuf, recvbuflen, 0);
        if(iResult > 0) {
            printf("message received: %s", recvbuf);
        }
        else if (iResult == 0) {
            linebreak();
            printf("connection closed...");
        }
        else
            printf("recv failed with error: %d", WSAGetLastError());

    } while(iResult > 0 );
    

    // Disabling sends or receives on a socket
    iResult = shutdown(connectSocket, SD_SEND);
    if(iResult == SOCKET_ERROR) {
        printf("shutdown failed with error %d\n", WSAGetLastError());
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
        return 1;
    } 

    closesocket(connectSocket);
    WSACleanup();

    return 0;

}

