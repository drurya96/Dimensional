#include <benchmark/benchmark.h>
#include "StaticLength.h"

static void BM_Multiply_Static_Lengths(benchmark::State& state)
{
   for (auto _ : state)
   {
      StaticDimension::Length<StaticDimension::Meters> length1{10.0};
      StaticDimension::Length<StaticDimension::Feet> length2{20.0};

      auto result = length1 * length2;
      benchmark::DoNotOptimize(result);
   }
}
BENCHMARK(BM_Multiply_Static_Lengths);


BENCHMARK_MAIN();