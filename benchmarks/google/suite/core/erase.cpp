// Vector Erase Benchmark Suite
// Measures erase() performance against std::vector.
//
// Covers:
// - erase (front, middle, end)

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// erase front
// ---------------------------------------------------------------------------

static void EraseFront_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(500, 0);
        (void)v.erase(v.cbegin());
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EraseFront_VectorPro);

static void EraseFront_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(500, 0);
        v.erase(v.cbegin());
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EraseFront_StdVector);

// ---------------------------------------------------------------------------
// erase middle
// ---------------------------------------------------------------------------

static void EraseMiddle_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(500, 0);
        (void)v.erase(v.cbegin() + 250);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EraseMiddle_VectorPro);

static void EraseMiddle_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(500, 0);
        v.erase(v.cbegin() + 250);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EraseMiddle_StdVector);

// ---------------------------------------------------------------------------
// erase end
// ---------------------------------------------------------------------------

static void EraseEnd_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(500, 0);
        (void)v.erase(v.cend() - 1);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EraseEnd_VectorPro);

static void EraseEnd_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(500, 0);
        v.erase(v.cend() - 1);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EraseEnd_StdVector);
