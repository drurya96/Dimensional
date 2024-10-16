cmake_minimum_required(VERSION 3.27)

include(UnitTests.cmake)

target_link_libraries(Dimension_UnitTests PUBLIC
    Dimension_LIB
    Dimension_Extensions
    gtest
    gtest_main
)
