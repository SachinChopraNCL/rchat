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
#include "formatting.h"
#include "globals.h"

class client {
public:
    client(char* port, char* ip):_port(port), _ip(ip){}; 
    void start_session();
    void end_session(); 
private: 
    void initialise_wsa(); 
    void find_server_connect(); 
    void send_handler();
    void receive_handler();
    void kick_threads();

    WSADATA _wsa_data; 
    SOCKET _server  = INVALID_SOCKET; 

    // Addrinfo holds host address information
    struct addrinfo *_addr_results = NULL, 
                    *_ptr_to_addr = NULL,
                    _hints;

    int _result;
    char* _port; 
    char* _ip; 
};
#endif