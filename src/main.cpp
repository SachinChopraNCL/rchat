#include "client.h"
#include "formatting.h"

#define BUFLEN 512
#define PORT "27015"

int __cdecl main(int argc, char** argv){
    rchat::line();
    rchat::printstart("Client Interface");
    rchat::linebreak();


    if(argc != 2) {
        printf("\nusages: %s server-name\n", argv[0]);
        return 1;
    }

    Client client = Client(BUFLEN, PORT, argv[1]);

    client.StartSession();

    return 1; 
}

