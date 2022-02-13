#ifndef CLIENT_SOCKET_INFO_H 
#define CLIENT_SOCKET_INFO_H

#include <winsock2.h> 
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h> 
#include <thread>
#include <queue>
#include "globals.h"

class client_socket_info {
    public:
        client_socket_info(SOCKET client, unsigned int id): _client_socket(client), _id(id){
            // Kick receive thread 
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
#endif 
