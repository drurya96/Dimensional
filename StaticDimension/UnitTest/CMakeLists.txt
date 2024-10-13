cmake_minimum_required(VERSION 3.27)

include(UnitTests.cmake)

target_link_libraries(StaticDimension_UnitTests PUBLIC
    StaticDimension_LIB
    StaticDimension_Extensions
    gtest
    gtest_main
)
