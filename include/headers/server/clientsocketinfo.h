#ifndef CLIENT_SOCKET_INFO_H 
#define CLIENT_SOCKET_INFO_H

#include <winsock2.h> 
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h> 
#include <thread>
#include <queue>
#include <config.h>

namespace rchat {
    class client_socket_info {
        public:
            client_socket_info(SOCKET client, unsigned int id): _client_socket(client), _id(id){
                _is_active = true; 
                // Kick receive thread 
                _receive_ref = std::thread(&client_socket_info::receive_handler, this);
            }
            
            std::thread _receive_ref; 
            SOCKET _client_socket; 
            unsigned int _id;
            std::string _client_name = "user"; 
            bool _received_meta = false; 
            bool _is_active = false;
            std::queue<rchat::message> _message_queue; 
            std::mutex _m_message_queue_mutex;

        private:
            void receive_handler();
            rchat::message _msg; 
            
    };
}
#endif 
