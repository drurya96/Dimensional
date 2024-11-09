cmake_minimum_required(VERSION 3.25)
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

include(../Dimension/UnitTest/UnitTests.cmake)

target_link_libraries(Dimension_UnitTests PUBLIC)
target_compile_definitions(Dimension_UnitTests PRIVATE ${DIMENSIONAL_PrecisionType})
