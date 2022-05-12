add_executable(rchat main.cpp)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(console_process_handler)


target_link_libraries(rchat
    PUBLIC
    client
    server
    wsock32
    ws2_32
)
