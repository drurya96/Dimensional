cmake_minimum_required(VERSION 3.27)

add_library(StaticDimension_Extensions INTERFACE)

target_include_directories(StaticDimension_Extensions INTERFACE ${CMAKE_CURRENT_LIST_DIR})

#target_link_libraries(StaticDimension_Extensions INTERFACE StaticDimension_LIB)
#target_link_libraries(StaticDimension_Extensions INTERFACE ${CONAN_LIBS})
#target_link_libraries(StaticDimension_Extensions INTERFACE Dimensional::Dimensional)
