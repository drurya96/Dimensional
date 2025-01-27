cmake_minimum_required(VERSION 3.25)

enable_testing()

set(PHYSICS_SOURCES 
    Example1.cpp
)

add_executable(Dimension_PhysicsProblems ${PHYSICS_SOURCES})

target_include_directories(Dimension_PhysicsProblems PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(Dimension_PhysicsProblems PUBLIC
    Dimension_LIB
    gtest
    gtest_main
)
