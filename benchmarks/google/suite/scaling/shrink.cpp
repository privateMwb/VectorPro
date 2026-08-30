// Vector Shrink Benchmark Suite
// Measures the performance of shrink_to_fit() against equivalent
// std::vector operations.
//
// Covers:
// - shrink_to_fit after over-reserving capacity

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// shrink_to_fit
// ---------------------------------------------------------------------------

static void ShrinkToFit_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
        v.shrink_to_fit();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ShrinkToFit_VectorPro);

static void ShrinkToFit_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
        v.shrink_to_fit();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ShrinkToFit_StdVector);
