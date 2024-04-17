cmake_minimum_required(VERSION 3.15)

project(Dimensional)

set(Enable_Dimensional_Tests OFF CACHE BOOL "Enable this flag to run unit tests for the Dimensional library")

set(Dimensional_Sources
    UnitDefinitions.cpp
)

#add_library(Dimensional_LIB OBJECT ${Dimensional_Sources})
add_library(Dimensional_LIB INTERFACE)

target_include_directories(Dimensional_LIB INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(Enable_Dimensional_Tests)
    if(TARGET gtest)
        add_subdirectory(UnitTest)
    else()
        message(STATUS "Attempted to add unit tests without target gtest")
    endif()
endif()
