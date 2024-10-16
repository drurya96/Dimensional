cmake_minimum_required(VERSION 3.27)

enable_testing()

set(PHYSICS_SOURCES 
    PhysicsProblemsBase.cpp

    Example1.cpp
)

add_executable(StaticDimension_PhysicsProblems ${PHYSICS_SOURCES})

target_include_directories(StaticDimension_PhysicsProblems PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(StaticDimension_PhysicsProblems PUBLIC
    StaticDimension_LIB
    gtest
    gtest_main
)
