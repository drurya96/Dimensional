cmake_minimum_required(VERSION 3.25)

enable_testing()

include(${CMAKE_CURRENT_LIST_DIR}/../ExampleExtensions/CMakeLists.txt)

set(TEST_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/DimensionTest.cpp

    ${CMAKE_CURRENT_LIST_DIR}/TestUtilities.cpp

    ${CMAKE_CURRENT_LIST_DIR}/TestFundamentalConversions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestCompoundConversions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestOperators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestSimplification.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestCastOperators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestNewDimension.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestStd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestTrig.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestFunctions.cpp
)

add_executable(Dimension_UnitTests ${TEST_SOURCES})

target_include_directories(Dimension_UnitTests PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(Dimension_UnitTests PUBLIC
    Dimension_Extensions
    gtest
    gtest_main
)

if (MSVC)
    target_compile_options(Dimension_UnitTests PRIVATE /W4 /WX)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Dimension_UnitTests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
