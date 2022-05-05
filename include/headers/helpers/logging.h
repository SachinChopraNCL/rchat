#ifndef LOGGING_H 
#define LOGGING_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <map> 

namespace rchat { 

    inline void line(int number_of_lines = 1) { 
        for(int i = 0; i < number_of_lines; i++) {
            std::cout << "--------------------------------" << '\n'; 
        }
    }

    enum message_type { 
        CONSOLE, 
        START,
        SESSION,
        ERR,
        INPUT,
        NOTAG
    };

    const std::map<message_type, std::string> enums_as_string {
        {message_type::CONSOLE, "[CONSOLE]"},
        {message_type::START, "[START]"},
        {message_type::SESSION, "[SESSION]"},
        {message_type::INPUT, "[SESSION]"},
        {message_type::ERR, "[ERROR]"}
    };

    inline std::string get_enum_as_string(message_type e) { 
        auto it = enums_as_string.find(e);
        return it == enums_as_string.end() ? "[INVALID LOG TYPE]" : it->second;
    }

    template<typename... t>
        static void log(message_type type = message_type::NOTAG, t&&... args) { 
            if(type != message_type::NOTAG) {
                std::cout << get_enum_as_string(type) << " ";
            }
            // voodoo fold statement
            ((std::cout << args << ' '), ...);
            if(type != message_type::INPUT) { std::cout << '\n'; }
        }
}

#endif 