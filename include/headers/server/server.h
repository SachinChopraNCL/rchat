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
#include <vector>
#include <queue>
#include <mutex> 
#include <algorithm>

#include "formatting.h"
#include "globals.h"
#include "clientsocketinfo.h"


class server {
public: 
    server(int buflen, char* port):_buflen(buflen), _port(port){};
    void start_session(); 
    void end_session(); 
private:
    void initialise_wsa();
    void prune_clients(); 
    void create_listener(); 
    void activate_listener(); 
    void accept_connection();
    void broadcast_handler(); 
    void kick_threads(); 

    WSADATA _wsa_data; 
    
    SOCKET _listener  = INVALID_SOCKET;
    SOCKET _client_socket = INVALID_SOCKET;

    std::vector<client_socket_info*> _clients; 
    std::queue<rchat::message> _global_message_cache; 

    struct addrinfo* _addr_results = NULL;
    struct addrinfo _hints; 

    bool _ready_to_send = false;

    int _result; 
    unsigned int _buflen; 
    char* _port; 

    int _client_id = 0; 

};
#endif