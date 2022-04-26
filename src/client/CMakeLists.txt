add_library(
     client OBJECT
     client_session.cpp 
     client_session_handler.cpp 
)

target_include_directories( client PUBLIC 
                            ${CMAKE_SOURCE_DIR}/include/headers/client
                            ${CMAKE_SOURCE_DIR}/src/helpers
                         )


