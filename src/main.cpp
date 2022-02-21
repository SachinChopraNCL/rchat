#include "client.h"
#include "server.h"
#include "formatting.h"

#define BUFLEN 512
#define PORT "27015"

int __cdecl main(int argc, char** argv){
    
    std::string client_str("client");

    if(client_str.compare(argv[1]) == 0){
        if(argc == 3) {
            rchat::line();
            rchat::printstart("Client Interface");
            rchat::linebreak();
            client client_process = client(PORT, argv[2]);
            client_process.start_session();
        }
        else {
            rchat::printerrori("INVALID NUMBER OF ARGUMENTS FOR CLIENT SESSION... Expected 3 got", argc);
        }
    } 
    else {
        if(argc == 2) {
            rchat::line();
            rchat::printstart("Server Interface");
            rchat::linebreak();
            server server_process = server(BUFLEN, PORT);
            server_process.start_session();
        }
        else {
            rchat::printerrori("INVALID NUMBER OF ARGUMENTS FOR CLIENT SESSION... Expected 2 got", argc);
        }
    }


    return 1; 
}

