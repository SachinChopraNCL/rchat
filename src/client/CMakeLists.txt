add_library(
     client OBJECT
     client_session.cpp 
     client_session_handler.cpp 
)

target_include_directories( client PUBLIC 
                            ${CMAKE_SOURCE_DIR}/include/headers/console_process_handler
                            ${CMAKE_SOURCE_DIR}/include/headers/client
                            ${CMAKE_SOURCE_DIR}/include/config
                            ${CMAKE_SOURCE_DIR}/include/headers/helpers
                         )


target_link_libraries(client PUBLIC console_process_handler)