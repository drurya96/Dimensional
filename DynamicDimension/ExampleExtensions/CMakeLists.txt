cmake_minimum_required(VERSION 3.15)

add_library(Extensions INTERFACE)

target_include_directories(Extensions INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(Extensions INTERFACE DynamicDimension_LIB)
