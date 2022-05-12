add_library(console_process_handler console_process_handler.cpp)


set(Boost_DEBUG ON)
set(BOOST_ROOT "C:/local/boost_1_79_0/boost")
set(Boost_INCLUDE_DIR "C:/local/boost_1_79_0")
set(BOOST_LIBRARYDIR "C:/local/boost_1_79_0/stage/lib")
set(Boost_ARCHITECTURE "-x64")
find_package(Boost REQUIRED COMPONENTS filesystem system)

target_include_directories(console_process_handler PUBLIC 
                          ${CMAKE_SOURCE_DIR}/include/headers/console_process_handler
                          ${CMAKE_SOURCE_DIR}/include/headers/helpers
                          ${CMAKE_SOURCE_DIR}/include/config
                          ${Boost_INCLUDE_DIR}
                        )

target_link_libraries(console_process_handler PUBLIC Boost::filesystem Boost::system)