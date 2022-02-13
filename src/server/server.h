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
#include "formatting.h"
#include "globals.h"

#define MAX_CLIENTS 64 

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
    void broadcast_handler(); 
    void kick_threads(); 

    // Hold client information 
    class client_socket_info {
    public:
        client_socket_info(SOCKET client, unsigned int id): _client_socket(client), _id(id){
            _receive_ref = std::thread(&client_socket_info::receive_handler, this);
        }
        
        std::thread _receive_ref; 
        SOCKET _client_socket; 
        unsigned int _id;
        std::queue<rchat::message> _message_queue; 

    private:
        void receive_handler();
        rchat::message _msg; 
    };

    WSADATA _wsa_data; 
    
    SOCKET _listener  = INVALID_SOCKET;
    SOCKET _client_socket = INVALID_SOCKET;

    std::vector<client_socket_info*> _clients; 

    struct addrinfo* _addr_results = NULL;
    struct addrinfo _hints; 

    bool _ready_to_send = false;

    int _result; 
    unsigned int _buflen; 
    char* _port; 
};
#endif