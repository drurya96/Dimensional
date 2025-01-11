cmake_minimum_required(VERSION 3.25)

add_library(Dimension_LIB INTERFACE)

target_include_directories(Dimension_LIB INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/FundamentalDimensions
    ${CMAKE_CURRENT_SOURCE_DIR}/DerivedDimensions
)

target_compile_definitions(Dimension_LIB INTERFACE ${DIMENSIONAL_PrecisionType})

if(DIMENSIONAL_REQUIRE_CONVERSIONS)
    target_compile_definitions(Dimension_LIB INTERFACE REQUIRE_CONVERSIONS)
endif()

if(Enable_Dimensional_Tests)
    #add_subdirectory(ExampleExtensions)
    if (NOT TARGET gtest)
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
        )
        # Fetch the content
        FetchContent_MakeAvailable(googletest)
    endif()
    add_subdirectory(UnitTest)
endif()
