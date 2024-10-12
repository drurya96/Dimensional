cmake_minimum_required(VERSION 3.27)
project(test_package CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include Conan-generated build info
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Set precision type as a compile definition, default to USE_DOUBLE if not set
if(NOT DEFINED DIMENSIONAL_PrecisionType)
    set(DIMENSIONAL_PrecisionType USE_DOUBLE CACHE STRING "Precision type")
endif()

# Add test_package executable
add_executable(test_package test_package.cpp)

# Pass precision type as a compile definition (i.e., -DUSE_DOUBLE)
target_compile_definitions(test_package PRIVATE ${DIMENSIONAL_PrecisionType})

# Link with Conan-provided libraries (this will just provide headers in your case)
target_link_libraries(test_package ${CONAN_LIBS})
