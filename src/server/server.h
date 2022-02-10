#ifndef SERVER_H
#define SERVER_H

/* For mingw compiler */
#define _WIN32_WINNT 0x501

#include <winsock2.h> 
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h> 
#include <thread>
#include "formatting.h"

class server {
public: 
    server(int buflen, char* port):_buflen(buflen), _port(port){};
    void start_session(); 
    void end_session(); 
private:
    void initialise_wsa();
    void create_listener(); 
    void activate_listener(); 
    void accept_connection();
    void send_handler();
    void receive_handler(); 
    void kick_threads(); 

    WSADATA _wsa_data; 
    
    SOCKET _listener  = INVALID_SOCKET;
    SOCKET _client = INVALID_SOCKET; 

    struct addrinfo* _addr_results = NULL;
    struct addrinfo _hints; 

    bool _ready_to_send = false;

    int _result; 
    unsigned int _buflen; 
    char* _port; 
};
#endif