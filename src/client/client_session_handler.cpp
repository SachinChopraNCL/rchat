#include <client_session_handler.h>
#include <client_session.h>
#include <logging.h> 

using namespace rchat;

void client_session_handler::run() {
    line();
    log(message_type::CONSOLE, "Welcome to RChat Client... Enter command INFO for Commands"); 
    line();
    while(_current_state != session_state::EXIT) {
        if(_current_state == session_state::RESET) {
            line(2);
            log(message_type::CONSOLE, "Welcome back...");
            line();
            _current_state = session_state::MENU;
        }
        if(_current_state == session_state::MENU) {
            log(message_type::INPUT, "Enter Command: ");
            std::string option;
            std::getline(std::cin , option); 
            run_command(option);
        }
    }
}

void client_session_handler::run_command(std::string option) { 
    std::vector<std::string> parameters = get_parameters(option);
    if(option.find(_commands[0]) != std::string::npos) {
        run_session(parameters[1]);
    }
    else if(option.find(_commands[1]) != std::string::npos) {
        set_port(parameters[1]);
    }
    else if(option.find(_commands[2]) != std::string::npos) {
        get_info();
    }
    else {
        log(message_type::ERR, "Option not recognised!");
    }
}

void client_session_handler::run_session(std::string ip) {
    line(2);  
    auto current_session = std::make_unique<client_session>(_port.c_str(), ip.c_str()); 
    current_session->start_session();
    _current_state = session_state::RESET; 
}

void client_session_handler::exit_session() { 
    
}

void client_session_handler::get_info() { 
    line();
    log(message_type::CONSOLE, "CONNECT x - Connect to server with IP x");
    log(message_type::CONSOLE, "SETPORT x - Set port to connect on server to X");
    log(message_type::CONSOLE, "Current Port -", _port);
    line();
}

void client_session_handler::set_port(std::string option) {
    line();
    log(message_type::CONSOLE, "Port was", _port, "Now set to", option);
    _port = option;
    line();
}

std::vector<std::string> client_session_handler::get_parameters(const std::string option) {
    std::string option_to_split = option;   
    std::string delimiter = " ";
    std::vector<std::string> parameters {};
    int position = 0;
    while((position = option_to_split.find(delimiter)) != std::string::npos){
        parameters.push_back(option_to_split.substr(0, position));
        option_to_split.erase(0, position + delimiter.length());
    }

    parameters.push_back(option_to_split);
    
    return parameters; 
}