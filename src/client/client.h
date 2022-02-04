#ifndef CLIENT_H
#define CLIENT_H

/* For mingw compiler */
#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream> 
#include <thread>
#include "client.h"
#include "formatting.h"

class Client {
public:
    Client(int buflen, char* port, char* ip):_buflen(buflen), _port(port), _ip(ip){}; 
    void StartSession();
private: 
    void InitialiseWSA(); 
    void FindServerAndConnect(); 
    void SendHandler();
    void ReceiveHandler();
    void KickThreads();
    void EndSession(); 

    WSADATA wsaData; 
    SOCKET server  = INVALID_SOCKET; 

    // Addrinfo holds host address information
    struct addrinfo *addrResults = NULL, 
                    *ptrToAddr = NULL,
                    hints;

    int _result;
    unsigned int _buflen; 
    char* _port; 
    char* _ip; 
};

#endif