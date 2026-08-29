// Vector swap benchmark suite.
//
// Measures swap performance against std::vector.
//
// Coverage:
// - member swap()
// - free-function swap() (ADL)

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// member swap
// ---------------------------------------------------------------------------

static void MemberSwap_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> a(1000, 1);
        Vector<int> b(1000, 2);
        a.swap(b);
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(MemberSwap_VectorPro);

static void MemberSwap_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> a(1000, 1);
        std::vector<int> b(1000, 2);
        a.swap(b);
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(MemberSwap_StdVector);

// ---------------------------------------------------------------------------
// free swap
// ---------------------------------------------------------------------------

static void FreeSwap_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> a(1000, 1);
        Vector<int> b(1000, 2);
        swap(a, b);
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(FreeSwap_VectorPro);

static void FreeSwap_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> a(1000, 1);
        std::vector<int> b(1000, 2);
        swap(a, b);
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(FreeSwap_StdVector);
