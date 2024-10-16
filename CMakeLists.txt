cmake_minimum_required(VERSION 3.27)

project(Dimensional)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Check if Conan is available
find_program(CONAN_CMD conan)
if(NOT CONAN_CMD)
    message(FATAL_ERROR "Conan not found. Please install Conan.")
endif()

# Run Conan install
execute_process(
    COMMAND ${CONAN_CMD} install .. --output-folder ${CMAKE_BINARY_DIR} --build=missing
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE CONAN_RESULT
)

set(Enable_Dimensional_Tests OFF CACHE BOOL "Enable this flag to run unit tests for the Dimensional library")
set(Enable_Dimensional_Benchmarks OFF CACHE BOOL "Enable this flag to run benchmarks for the Dimensional library")
set(DIMENSIONAL_REQUIRE_CONVERSIONS OFF CACHE BOOL "Enable this flag to treat unspecialized conversions as compile-time errors")

if (NOT DEFINED DIMENSIONAL_PrecisionType)
    set(DIMENSIONAL_PrecisionType USE_DOUBLE CACHE STRING "This controls the PrecisionType of the library. Use one of USE_DOUBLE, USE_FLOAT, or USE_LONG_DOUBLE")
endif()

add_subdirectory(Dimension)
#add_subdirectory(Benchmark)
