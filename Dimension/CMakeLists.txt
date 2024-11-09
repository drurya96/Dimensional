cmake_minimum_required(VERSION 3.25)

add_library(Dimension_LIB INTERFACE)

target_include_directories(Dimension_LIB PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/FundamentalDimensions
    ${CMAKE_CURRENT_SOURCE_DIR}/DerivedDimensions
)

target_compile_definitions(Dimension_LIB INTERFACE ${DIMENSIONAL_PrecisionType})

if(DIMENSIONAL_REQUIRE_CONVERSIONS)
    target_compile_definitions(Dimension_LIB INTERFACE REQUIRE_CONVERSIONS)
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
