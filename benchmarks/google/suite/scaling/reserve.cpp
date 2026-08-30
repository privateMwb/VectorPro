// Vector Reserve Benchmark Suite
// Measures the performance of reserve() against equivalent std::vector
// operations.
//
// Covers:
// - reserve on an empty vector
// - reserve with repeated growth targets
// - reserve no-op (capacity already sufficient)

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// reserve
// ---------------------------------------------------------------------------

static void Reserve_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v;
        v.reserve(1000);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(Reserve_VectorPro);

static void Reserve_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(1000);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(Reserve_StdVector);

// ---------------------------------------------------------------------------
// reserve growth
// ---------------------------------------------------------------------------

static void ReserveGrowth_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v;
        for (std::size_t cap = 64; cap <= 4096; cap *= 2)
            v.reserve(cap);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ReserveGrowth_VectorPro);

static void ReserveGrowth_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        for (std::size_t cap = 64; cap <= 4096; cap *= 2)
            v.reserve(cap);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ReserveGrowth_StdVector);

// ---------------------------------------------------------------------------
// reserve no-op
// ---------------------------------------------------------------------------

static void ReserveNoop_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(2000);

    for (auto _ : state) {
        v.reserve(1000);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ReserveNoop_VectorPro);

static void ReserveNoop_StdVector(benchmark::State& state) {
    std::vector<int> v;
    v.reserve(2000);

    for (auto _ : state) {
        v.reserve(1000);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ReserveNoop_StdVector);
