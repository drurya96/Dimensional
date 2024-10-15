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

include(../StaticDimension/UnitTest/UnitTests.cmake)

message(STATUS "CONAN_LIBS contains: ${CONAN_LIBS}")

#target_link_libraries(StaticDimension_UnitTests PUBLIC ${CONAN_LIBS})
target_link_libraries(StaticDimension_UnitTests PUBLIC)
target_compile_definitions(StaticDimension_UnitTests PRIVATE ${DIMENSIONAL_PrecisionType})
