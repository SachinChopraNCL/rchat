add_library(
    server OBJECT
    server.cpp
)

target_include_directories(server PUBLIC 
                          ${CMAKE_SOURCE_DIR}/include/headers/server
                          ${CMAKE_SOURCE_DIR}/src/helpers
                          ${CMAKE_SOURCE_DIR}/include/config
                        )
