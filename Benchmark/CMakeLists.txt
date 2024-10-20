cmake_minimum_required(VERSION 3.25)

set(BENCHMARK_SOURCES
    ExampleBenchmark.cpp
)

add_executable(DimensionBenchmarkLib ${BENCHMARK_SOURCES})

target_include_directories(DimensionBenchmarkLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(DimensionBenchmarkLib PUBLIC
    Dimension_LIB
    Dimension_Extensions
    benchmark::benchmark
)
