#include <benchmark/benchmark.h>
#include "length_dimension.h"

static void BM_Multiply_Static_lengths(benchmark::State& state)
{
   for (auto _ : state)
   {
      dimension::length<dimension::meters> length1{10.0};
      dimension::length<dimension::Feet> length2{20.0};

      auto result = length1 * length2;
      benchmark::DoNotOptimize(result);
   }
}
BENCHMARK(BM_Multiply_Static_lengths);


BENCHMARK_MAIN();