// Vector Pop Clear Benchmark Suite
// Measures pop_back() and clear() performance against std::vector.
//
// Covers:
// - pop_back
// - clear

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// pop_back
// ---------------------------------------------------------------------------

static void PopBack_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(1000, 0);
        while (!v.empty())
            v.pop_back();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(PopBack_VectorPro);

static void PopBack_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(1000, 0);
        while (!v.empty())
            v.pop_back();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(PopBack_StdVector);

// ---------------------------------------------------------------------------
// clear
// ---------------------------------------------------------------------------

static void Clear_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(1000, 0);
        v.clear();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(Clear_VectorPro);

static void Clear_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(1000, 0);
        v.clear();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(Clear_StdVector);
