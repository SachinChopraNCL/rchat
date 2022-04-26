#ifndef GLOBALS_H 
#define GLOBALS_H

#include <stdio.h>

#define MAX_CLIENTS 64 


namespace rchat {

    // convert to Extern
    const int BUF_LEN = 64; 
    const int MAX_CMD_LEN = 100; 
    
    // todo take out 
    inline std::string EXIT("EXIT");

    struct message { 
        int  _message_length; 
        char _content [BUF_LEN];
    };

}


#endif 