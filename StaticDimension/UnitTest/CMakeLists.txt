cmake_minimum_required(VERSION 3.27)

enable_testing()

set(TEST_SOURCES 
    DimensionTest.cpp

    TestUtilities.cpp

    TestFundamentalConversions.cpp
    TestCompoundConversions.cpp
    TestOperators.cpp
    TestSimplification.cpp
    TestCastOperators.cpp
    TestNewDimension.cpp
)

add_executable(StaticDimension_UnitTests ${TEST_SOURCES})

target_include_directories(StaticDimension_UnitTests PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(StaticDimension_UnitTests PUBLIC
    StaticDimension_LIB
    StaticDimension_Extensions
    gtest
    gtest_main
)
