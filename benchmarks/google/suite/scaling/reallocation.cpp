// Vector Reallocation Benchmark Suite
// Measures the cost of a single growth-triggered reallocation, isolating
// the move/copy-and-free overhead from push_back() itself.
//
// Covers:
// - growth-triggered reallocation with a populated buffer

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// growth reallocation
// ---------------------------------------------------------------------------

static void GrowthReallocation_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(1024, 0);
        v.push_back(99); // size == capacity, forces one reallocation
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(GrowthReallocation_VectorPro);

static void GrowthReallocation_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(1024, 0);
        v.push_back(99);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(GrowthReallocation_StdVector);
