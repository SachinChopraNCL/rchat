#include "client.h"
#include "server.h"
#include "formatting.h"

#define BUFLEN 512
#define PORT "27015"

int __cdecl main(int argc, char** argv){
    
    std::string client_str("client");

    if(client_str.compare(argv[1]) == 0){
        rchat::line();
        rchat::printstart("Client Interface");
        rchat::linebreak();
        client client_process = client(BUFLEN, PORT, argv[1]);
        client_process.start_session();
    } 
    else {
        rchat::line();
        rchat::printstart("Server Interface");
        rchat::linebreak();
        server server_process = server(BUFLEN, PORT);
        server_process.start_session(); 
    }


    return 1; 
}

