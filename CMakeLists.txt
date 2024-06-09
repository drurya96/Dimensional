cmake_minimum_required(VERSION 3.15)

project(Dimensional)

set(Enable_Dimensional_Tests OFF CACHE BOOL "Enable this flag to run unit tests for the Dimensional library")
set(Enable_Dimensional_Benchmarks OFF CACHE BOOL "Enable this flag to run benchmarks for the Dimensional library")
set(DIMENSIONAL_REQUIRE_CONVERSIONS OFF CACHE BOOL "Enable this flag to treat unspecialized conversions as compile-time errors")

set(DIMENSIONAL_PrecisionType USE_DOUBLE CACHE STRING "This controls the PrecisionType of the library. Use one of USE_DOUBLE, USE_FLOAT, or USE_LONG_DOUBLE")

add_subdirectory(StaticDimension)
add_subdirectory(Benchmark)
