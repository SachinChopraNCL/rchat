#ifndef GLOBALS_H 
#define GLOBALS_H

#include <stdio.h>

#define MAX_CLIENTS 64 


namespace rchat {
    const int BUF_LEN = 64; 

    struct message { 
        int  _message_length; 
        char _content [BUF_LEN];
    };

}


#endif 