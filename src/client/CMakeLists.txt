add_library(clientimpl "")

target_sources(clientimpl 
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/client.cpp 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/client.h
)

target_include_directories(clientimpl PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_SOURCE_DIR}/src/helpers/)