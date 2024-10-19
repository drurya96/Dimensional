cmake_minimum_required(VERSION 3.25)

include(UnitTests.cmake)

target_link_libraries(Dimension_UnitTests PUBLIC
    Dimension_LIB
    Dimension_Extensions
    gtest
    gtest_main
)
