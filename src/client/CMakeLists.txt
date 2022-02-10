add_library(client "")

target_sources(client 
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/client.cpp 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/client.h
)

target_include_directories(client PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_SOURCE_DIR}/src/helpers/)