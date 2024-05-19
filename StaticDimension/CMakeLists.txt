cmake_minimum_required(VERSION 3.15)

add_library(StaticDimension_LIB INTERFACE)

target_include_directories(StaticDimension_LIB INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(Enable_Dimensional_Tests)
    add_subdirectory(ExampleExtensions)
    if(TARGET gtest)
        add_subdirectory(UnitTest)
    else()
        message(STATUS "Attempted to add unit tests without target gtest")
    endif()
endif()
