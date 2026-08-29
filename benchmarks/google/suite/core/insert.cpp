// Vector Insert Benchmark Suite
// Measures insert() performance against std::vector.
//
// Covers:
// - insert (front, middle, end)

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// insert front
// ---------------------------------------------------------------------------

static void InsertFront_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(500, 0);
        (void)v.insert(v.cbegin(), 99);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(InsertFront_VectorPro);

static void InsertFront_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(500, 0);
        v.insert(v.cbegin(), 99);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(InsertFront_StdVector);

// ---------------------------------------------------------------------------
// insert middle
// ---------------------------------------------------------------------------

static void InsertMiddle_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(500, 0);
        (void)v.insert(v.cbegin() + 250, 99);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(InsertMiddle_VectorPro);

static void InsertMiddle_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(500, 0);
        v.insert(v.cbegin() + 250, 99);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(InsertMiddle_StdVector);

// ---------------------------------------------------------------------------
// insert end
// ---------------------------------------------------------------------------

static void InsertEnd_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v(500, 0);
        (void)v.insert(v.cend(), 99);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(InsertEnd_VectorPro);

static void InsertEnd_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(500, 0);
        v.insert(v.cend(), 99);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(InsertEnd_StdVector);
