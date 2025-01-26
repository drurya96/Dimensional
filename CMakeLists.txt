cmake_minimum_required(VERSION 3.25)

project(Dimensional)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_CONAN "Enable Conan for dependency management" OFF)
option(Enable_Dimensional_Tests "Enable this flag to run unit tests for the Dimensional library" OFF)
option(Enable_Dimensional_Benchmarks "Enable this flag to run benchmarks for the Dimensional library" OFF)
option(DIMENSIONAL_REQUIRE_CONVERSIONS "Enable this flag to treat unspecialized conversions as compile-time errors" OFF)

if (USE_CONAN)
    # Check if Conan is available
    find_program(CONAN_CMD conan)
    if (CONAN_CMD)
        # Run Conan install
        execute_process(
            COMMAND ${CONAN_CMD} install .. --output-folder ${CMAKE_BINARY_DIR} --build=missing
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE CONAN_RESULT
        )
        if (NOT CONAN_RESULT EQUAL 0)
            message(FATAL_ERROR "Conan install failed. Please check your Conan configuration.")
        endif()

        # Include Conan-generated build information
        include(${CMAKE_BINARY_DIR}/conan_paths.cmake OPTIONAL)
    else()
        message(WARNING "Conan not found. Proceeding without Conan dependency management.")
    endif()
else()
    message(STATUS "Conan is disabled. Proceeding without Conan dependency management.")
endif()

if (NOT DEFINED DIMENSIONAL_PrecisionType)
    set(DIMENSIONAL_PrecisionType USE_DOUBLE CACHE STRING "This controls the PrecisionType of the library. Use one of USE_DOUBLE, USE_FLOAT, or USE_LONG_DOUBLE")
endif()

if(Enable_Dimensional_Tests)
    enable_testing()
endif()

add_subdirectory(Dimension)
#add_subdirectory(Benchmark)
