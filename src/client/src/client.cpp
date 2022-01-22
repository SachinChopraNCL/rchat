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
    printf("--------------------------------\n");
    printf("--------------------------------\n");
    printf("client interface");
    linebreak();

    WSADATA wsaData; 
    SOCKET server  = INVALID_SOCKET; 

    // Addrinfo holds host address information
    struct addrinfo  *addrResults = NULL, 
                     *ptrToAddr = NULL,
                     hints;

    char recvbuf[BUFLEN];
    int result;
    int recvbuflen = BUFLEN;

    if(argc != 2) {
        printf("\nusages: %s server-name\n", argv[0]);
        return 1;
    }

    printf("WSA is starting up...");
    
    // Initialise winsock2 
    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(result != 0) { 
        printf("WSAStartup failed with error: %d", result);
    }

    linebreak();

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    printf("getting server information...");

    // Translation from host name to an address getting all sockets that could be used for connection
    result = getaddrinfo(argv[1], PORT, &hints, &addrResults);
    if(result != 0) {
        printf("getaddrinfo failed with error: %d", result);
        WSACleanup();
        return 1;
    }

    // Check which server socket is valid and available
    for(ptrToAddr = addrResults; ptrToAddr != NULL;  ptrToAddr = ptrToAddr->ai_next) {
        server  = socket(ptrToAddr->ai_family, ptrToAddr->ai_socktype, ptrToAddr->ai_protocol);
        if(server  == INVALID_SOCKET) {
            printf("socket failed with error: %ld", WSAGetLastError());
            WSACleanup();
            return 1; 
        }

        // Attempt connection to the socket
        result = connect(server , ptrToAddr->ai_addr, (int)ptrToAddr->ai_addrlen);
        if(result == SOCKET_ERROR) {
            closesocket(server );
            server  = INVALID_SOCKET; 
            continue;
        }
        printf("connected!");
        break;
    }
    
    linebreak();
    freeaddrinfo(addrResults);

    if(server  == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1; 
    }

    printf("connection session is progress - id 0\n");

    std::string sendbuf;
    printf("send message: ");
    std::cin >> sendbuf;

    result = send(server , sendbuf.c_str(), (int)strlen(sendbuf.c_str()), 0);
    if(result == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(server );
        WSACleanup();
        return 1;
    }
    
    do {
        result = recv(server , recvbuf, recvbuflen, 0);
        if(result > 0) {
            printf("message received: %s", recvbuf);
        }
        else if (result == 0) {
            linebreak();
            printf("connection closed...");
        }
        else
            printf("recv failed with error: %d", WSAGetLastError());

    } while(result > 0 );
    

    // Disabling sends or receives on a socket
    result = shutdown(server , SD_SEND);
    if(result == SOCKET_ERROR) {
        printf("shutdown failed with error %d\n", WSAGetLastError());
        closesocket(server );
        server  = INVALID_SOCKET;
        return 1;
    } 

    closesocket(server);
    WSACleanup();

    return 0;

}

