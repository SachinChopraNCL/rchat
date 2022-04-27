#ifndef LOGGING_H 
#define LOGGING_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <map> 

namespace rchat { 

    //inline void line() { printf("--------------------------------\n"); }
    enum message_type { 
        CONSOLE, 
        START,
        SESSION,
        ERR
    };

    const std::map<message_type, std::string> enums_as_string {
        {message_type::CONSOLE, "[CONSOLE]"},
        {message_type::START, "[START]"},
        {message_type::SESSION, "[SESSION]"},
        {message_type::ERR, "[ERR]"}
    };

    std::string get_enum_as_string(message_type e) { 
        auto it = enums_as_string.find(e);
        return it == enums_as_string.end() ? "[INVALID LOG TYPE]" : it->second;
    }

    template<typename... t>
        static void log(message_type type ,t&&... args) { 
            std::cout << get_enum_as_string(type) << " ";
            ((std::cout << args << ' '), ...) << "\n";
        }
}

#endif 