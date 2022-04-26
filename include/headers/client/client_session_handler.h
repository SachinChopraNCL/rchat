#ifndef CLIENT_SESSION_HANDLER_H
#define CLIENT_SESSION_HANDLER_H

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
/* For mingw compiler */
#define _WIN32_WINNT 0x501
#define NUM_COMMANDS 4


#define DEFAULT_PORT "27015"

enum session_state {
    NULL_STATE,
    MENU,
    IN_SESSION, 
    EXIT
};


class client_session_handler {
public: 
    client_session_handler(){};
    void run(); 

private: 
    void run_session(std::string ip); 
    void set_port(std::string port);
    void get_info(); 
    void exit_session();
    void reset_session(); 
    void run_command(const std::string option);
    std::vector<std::string> get_parameters(std::string option);
    std::string _commands [NUM_COMMANDS] = {"CONNECT", "SETPORT", "INFO", "EXIT"};
    session_state _current_state = session_state::NULL_STATE;

    std::string _port = DEFAULT_PORT; 
};

#endif