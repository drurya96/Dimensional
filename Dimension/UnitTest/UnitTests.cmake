cmake_minimum_required(VERSION 3.25)

enable_testing()

include(${CMAKE_CURRENT_LIST_DIR}/../ExampleExtensions/CMakeLists.txt)

set(TEST_SOURCES
    
#[===[
]===]

    ${CMAKE_CURRENT_LIST_DIR}/TestFundamentalConversions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestCompoundConversions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestOperators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestSimplification.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestCastOperators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestStd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestTrig.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestFunctions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestSymbols.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PhysicsProblemsExamples/Example1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestSerialization.cpp

    #${CMAKE_CURRENT_LIST_DIR}/TestNewDimension.cpp


    #${CMAKE_CURRENT_LIST_DIR}/TestQuantityAdditionSubtraction.cpp
    #${CMAKE_CURRENT_LIST_DIR}/TestUtilities.cpp
    
    ${CMAKE_CURRENT_LIST_DIR}/TestTemp.cpp
)

add_executable(Dimension_UnitTests ${TEST_SOURCES})

target_include_directories(Dimension_UnitTests PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Dimension_UnitTests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/PhysicsProblemsExamples)

target_link_libraries(Dimension_UnitTests PUBLIC
    Dimension_Extensions
    gtest
    gtest_main
)

if (MSVC)
    target_compile_options(Dimension_UnitTests PRIVATE /W4 /WX)
    #target_compile_definitions(Dimension_UnitTests PRIVATE _CRT_NO_TIME_T)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Dimension_UnitTests PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror

        # Catch silent bugs
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wnull-dereference
        -Wdouble-promotion
        -Wcast-align
        -Wformat=2
        -Wimplicit-fallthrough
        -Wmissing-declarations
        -Wredundant-move
        -Wunreachable-code

        # Modern template hygiene
        #-Wctad-maybe-unsupported # TODO: Enable this and deal with warnings
)
endif()

include(GoogleTest)
gtest_discover_tests(Dimension_UnitTests)
