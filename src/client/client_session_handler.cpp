#include <client_session_handler.h>
#include <client_session.h>

void client_session_handler::run() {
    rchat::printconsole("Welcome to RChat Client..."); 
    while(_current_state != session_state::EXIT) {
        std::string option {"NULL"};
        std::getline(std::cin, option); 
        run_command(option);
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
        rchat::printerror("Option not recognised!");
    }
}

void client_session_handler::run_session(std::string ip) {
    rchat::linebreak(); 
    rchat::linebreak(); 
    auto current_session = std::make_unique<client_session>(_port.c_str(), ip.c_str()); 
    current_session->start_session(); 
}

void client_session_handler::exit_session() { 
    
}

void client_session_handler::reset_session() { 

}

void client_session_handler::get_info() { 
    char message[rchat::MAX_CMD_LEN];
    sprintf(message, "Port - %s", _port);
    rchat::printconsole(message);
}

void client_session_handler::set_port(std::string option) {
    char message[rchat::MAX_CMD_LEN]; 
    sprintf(message, "Port was %s. Now set to %s", _port, option);
    rchat::printconsole(message);
    _port = option;
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