add_library(helpers "")

target_sources(helpers PUBLIC ${CMAKE_CURRENT_LIST_DIR}/formatting.h ${CMAKE_CURRENT_LIST_DIR}/globals.h)

set_target_properties(helpers PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(helpers PUBLIC ${CMAKE_CURRENT_LIST_DIR})