cmake_minimum_required(VERSION 3.18)

project(rchat)

add_executable(rcclient src/client/src/client.cpp)

add_executable(rcserver src/server/src/server.cpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/src/") 

find_library(WS2_32_LIBRARY ws2_32)
find_library(WSOCK32_LIBRARY wsock32)

target_link_libraries(rcclient wsock32 ws2_32)
target_link_libraries(rcserver wsock32 ws2_32)