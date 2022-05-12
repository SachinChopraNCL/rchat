#include <console_process_handler.h>
using namespace rchat;

console_process_handler* console_process_handler::instance = nullptr;

console_process_handler::console_process_handler(const char* path){ 
    child_process = bp::child(path, "OPTIONAL", new_console(), bp::std_in < input_stream,
                        bp::std_out = output_stream, bp::std_err=stderr);
}

void console_process_handler::print_to_process(std::string string) { 
    input_stream << string << std::endl; 
}

void console_process_handler::kill_instance() { 
    child_process.terminate();
    delete instance; 
    instance = nullptr;
}