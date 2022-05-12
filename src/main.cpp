#include <client_session_handler.h>
#include <server.h>
#include <logging.h>
#include <iostream>


#define BUFLEN 512
#define PORT "27015"

using namespace rchat; 

int __cdecl main(int argc, char** argv){

    std::string client_str("client");

    if(client_str.compare(argv[1]) == 0){

        client_session_handler* handler = new client_session_handler();
        handler->run();  
    } 
    else {
        if(argc == 2) {
            server server_process = server(BUFLEN, PORT);
            server_process.start_session();
        }
        else {
            log(message_type::ERR,"INVALID NUMBER OF ARGUMENTS FOR CLIENT SESSION... Expected 2 got", argc);
        }
    }


    return 1; 
}