#include <client_session_handler.h>
#include <server.h>
#include <logging.h>
#include <iostream>


using namespace rchat; 

int __cdecl main(int argc, char** argv){

    std::string client_str("client");

    if(client_str.compare(argv[1]) == 0){

        client_session_handler* handler = new client_session_handler();
        handler->run();  
    } 
    else {
        
        std::string port = (argc == 3) ?  argv[2] : "27015";
        server server_process = server(rchat::global_network_variables::buflen, port.c_str());
        server_process.start_session();
        
    
    }


    return 1; 
}