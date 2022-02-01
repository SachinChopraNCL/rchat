/* For mingw compiler */
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream> 

#include "formatting.h"

/* For Visual Studio Compilers */
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
/*----------------------------*/

#define BUFLEN 512
#define PORT "27015"


int __cdecl main(int argc, char** argv){
    
    rchat::line();
    rchat::printstart("Client Interface");
    rchat::linebreak();

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

    rchat::printstart("WSA is starting up...");
    
    // Initialise winsock2 
    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(result != 0) { 
        rchat::printerrori("WSAStartup failed with error", result);
    }

    rchat::linebreak();

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

     rchat::printstart("Getting server information...");

    // Translation from host name to an address getting all sockets that could be used for connection
    result = getaddrinfo(argv[1], PORT, &hints, &addrResults);
    if(result != 0) {
        rchat::printerrori("Getaddrinfo failed with error", result);
        WSACleanup();
        return 1;
    }

    // Check which server socket is valid and available
    for(ptrToAddr = addrResults; ptrToAddr != NULL;  ptrToAddr = ptrToAddr->ai_next) {
        server  = socket(ptrToAddr->ai_family, ptrToAddr->ai_socktype, ptrToAddr->ai_protocol);
        if(server  == INVALID_SOCKET) {
            rchat::printerrorld("Socket failed with error", WSAGetLastError());
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
        printf("Connected!");
        break;
    }
    
    freeaddrinfo(addrResults);

    if(server  == INVALID_SOCKET) {
        rchat::printerror("Unable to connect to server!");
        WSACleanup();
        return 1; 
    }

    rchat::linebreak();

    rchat::printsession("Connection session is progress - id 0");

    
    // 0 connection ended 
    
    while(true) {

        // handle input 
        std::string sendbuf;
        rchat::printsession("Send message: ");
        std::cin >> sendbuf;

        result = send(server , sendbuf.c_str(), (int)strlen(sendbuf.c_str()), 0);
        if(result == SOCKET_ERROR) {
            rchat::printerrorld("Send failed with error", WSAGetLastError());
            closesocket(server );
            WSACleanup();
            return 1;
        }
        
        result = recv(server , recvbuf, recvbuflen, 0);
        if(result > 0) { 
            rchat::fprintsession("Server Response", recvbuf); 
        }
        else if(result == 0){}
        else { 
            rchat::printerrorld("Recv failed with error", WSAGetLastError()); 
        }
    }
    
    // Disabling sends or receives on a socket
    result = shutdown(server , SD_SEND);
    if(result == SOCKET_ERROR) {
        rchat::printerrorld("Shutdown failed with error", WSAGetLastError());
        closesocket(server);
        server  = INVALID_SOCKET;
        return 1;
    } 

    closesocket(server);
    WSACleanup();

    return 0;

}

