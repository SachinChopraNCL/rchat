add_library(server "")

target_sources(server 
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/server.cpp 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/server.h
)

target_include_directories(server PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_SOURCE_DIR}/src/helpers/)
