#ifndef CLIENT_SESSION_H
#define CLIENT_SESSION_H

#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream> 
#include <thread>
#include <config.h>
#include <console_process_handler.h>

namespace rchat { 
    class client_session {
    public:
        client_session(const char* port, const char* ip, std::string session_name ):_port(port), _ip(ip), _session_name(session_name){}; 
        void start_session();
        void end_session(); 
    private: 
        void initialise_wsa(); 
        void find_server_connect(); 
        void send_handler();
        void receive_handler();
        void kick_threads();
        void send_metadata(); 

        WSADATA _wsa_data; 
        SOCKET _server  = INVALID_SOCKET; 

        // Addrinfo holds host address information
        struct addrinfo *_addr_results = NULL, 
                        *_ptr_to_addr = NULL,
                        _hints;

        int _result;
        const char* _port; 
        const char* _ip; 
        
        bool _end_session = false;

        std::string _session_name = "user"; 

        std::thread _receive_ref; 
        std::thread _send_ref;     
    };
}
#endif