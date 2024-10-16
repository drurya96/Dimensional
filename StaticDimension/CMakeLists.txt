cmake_minimum_required(VERSION 3.27)

add_library(StaticDimension_LIB INTERFACE)

target_include_directories(StaticDimension_LIB INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(StaticDimension_LIB INTERFACE ${DIMENSIONAL_PrecisionType})

if(DIMENSIONAL_REQUIRE_CONVERSIONS)
    target_compile_definitions(StaticDimension_LIB INTERFACE REQUIRE_CONVERSIONS)
endif()

if(Enable_Dimensional_Tests)
    add_subdirectory(ExampleExtensions)
    if(TARGET gtest)
        add_subdirectory(UnitTest)
        add_subdirectory(PhysicsProblemsExamples)
    else()
        message(STATUS "Attempted to add unit tests without target gtest")
    endif()
endif()
