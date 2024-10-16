#include <benchmark/benchmark.h>
#include "Length.h"

static void BM_Multiply_Static_Lengths(benchmark::State& state)
{
   for (auto _ : state)
   {
      Dimension::Length<Dimension::Meters> length1{10.0};
      Dimension::Length<Dimension::Feet> length2{20.0};

      auto result = length1 * length2;
      benchmark::DoNotOptimize(result);
   }
}
BENCHMARK(BM_Multiply_Static_Lengths);


BENCHMARK_MAIN();