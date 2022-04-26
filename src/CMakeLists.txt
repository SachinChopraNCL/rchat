add_executable(rchat main.cpp)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(helpers)


target_link_libraries(rchat
    PUBLIC
    client
    server
    helpers
    wsock32
    ws2_32
)
