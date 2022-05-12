#ifndef CONSOLE_PROCESS_HANDLER_H
#define CONSOLE_PROCESS_HANDLER_H

#include <boost/process.hpp>
#include <boost/process/extend.hpp>

namespace bp = boost::process; 

namespace rchat { 
    // Singleton class 
    class console_process_handler { 
    public: 
        static console_process_handler* instance; 

        static console_process_handler* get_instance(const char* path = "") {
            if(instance == nullptr) {
                instance = new console_process_handler(path);
            }
            return instance; 
        }

        void print_to_process(std::string string);
        void kill_instance();

    private: 
        struct new_console : bp::extend::handler {
            template <typename Sequence>
            void on_setup(bp::extend::windows_executor<char, Sequence> & ex) {
                ex.creation_flags |= CREATE_NEW_CONSOLE;
            }
        };

        console_process_handler(const char* path); 

        bp::pstream output_stream; 
        bp::opstream input_stream; 
        bp::child child_process;
    };
}
#endif

