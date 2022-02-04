add_library(helpersimpl "")

target_sources(helpersimpl PUBLIC ${CMAKE_CURRENT_LIST_DIR}/formatting.h)

set_target_properties(helpersimpl PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(helpersimpl PUBLIC ${CMAKE_CURRENT_LIST_DIR})