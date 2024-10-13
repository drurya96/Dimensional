cmake_minimum_required(VERSION 3.27)

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
)

add_executable(StaticDimension_UnitTests ${TEST_SOURCES})

target_include_directories(StaticDimension_UnitTests PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(StaticDimension_UnitTests PUBLIC
    StaticDimension_Extensions
    gtest
    gtest_main
)
