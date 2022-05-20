#ifndef CONFIG_H 
#define CONFIG_H

#include <stdio.h>
#include <string>

namespace rchat {
    
    class global_network_variables { 
    public:
        static const int buflen = 64; 
        static const int max_cmd_len = 64; 
        inline static const std::string exit = "EXIT";
        inline static const std::string default_port = "27015";
    };

        
    struct message { 
        int  _message_length; 
        char _content [global_network_variables::buflen];
    };


}

#endif 