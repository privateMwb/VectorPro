// Vector Construction Benchmark Suite
// Measures construction performance against std::vector.
//
// Covers:
// - default construction
// - fill construction
// - initializer list construction

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// default construct
// ---------------------------------------------------------------------------

static void DefaultConstruct_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v;
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(DefaultConstruct_VectorPro);

static void DefaultConstruct_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(DefaultConstruct_StdVector);

// ---------------------------------------------------------------------------
// fill construct
// ---------------------------------------------------------------------------

static void FillConstruct_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(100, 0);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(FillConstruct_VectorPro);

static void FillConstruct_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(100, 0);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(FillConstruct_StdVector);

// ---------------------------------------------------------------------------
// init list construct
// ---------------------------------------------------------------------------

static void InitListConstruct_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v{1, 2, 3, 4, 5};
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(InitListConstruct_VectorPro);

static void InitListConstruct_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v{1, 2, 3, 4, 5};
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(InitListConstruct_StdVector);
