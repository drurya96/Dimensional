cmake_minimum_required(VERSION 3.27)

add_library(Dimension_Extensions INTERFACE)

target_include_directories(Dimension_Extensions INTERFACE ${CMAKE_CURRENT_LIST_DIR})

#target_link_libraries(Dimension_Extensions INTERFACE Dimension_LIB)
#target_link_libraries(Dimension_Extensions INTERFACE ${CONAN_LIBS})
#target_link_libraries(Dimension_Extensions INTERFACE Dimensional::Dimensional)
