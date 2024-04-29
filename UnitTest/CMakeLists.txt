cmake_minimum_required(VERSION 3.15)

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

add_executable(UnitTests ${TEST_SOURCES})

target_include_directories(UnitTests PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(UnitTests PUBLIC
    Dimensional_LIB
    Extensions
    gtest
    gtest_main
)
