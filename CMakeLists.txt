cmake_minimum_required(VERSION 3.15)

project(Dimensional)

set(Enable_Dimensional_Tests OFF CACHE BOOL "Enable this flag to run unit tests for the Dimensional library")

add_subdirectory(DynamicDimension)
add_subdirectory(StaticDimension)
