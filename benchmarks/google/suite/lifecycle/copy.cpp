// Vector Copy Benchmark Suite
// Measures copy construction and copy assignment performance against std::vector.
//
// Covers:
// - copy construction
// - copy assignment

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// copy construct
// ---------------------------------------------------------------------------

static void CopyConstruct_VectorPro(benchmark::State& state) {
    Vector<int> src(1000, 7);

    for (auto _ : state) {
        Vector<int> c(src);
        benchmark::DoNotOptimize(c);
    }
}
BENCHMARK(CopyConstruct_VectorPro);

static void CopyConstruct_StdVector(benchmark::State& state) {
    std::vector<int> stdSrc(1000, 7);

    for (auto _ : state) {
        std::vector<int> c(stdSrc);
        benchmark::DoNotOptimize(c);
    }
}
BENCHMARK(CopyConstruct_StdVector);

// ---------------------------------------------------------------------------
// copy assignment
// ---------------------------------------------------------------------------

static void CopyAssignment_VectorPro(benchmark::State& state) {
    Vector<int> src(1000, 7);
    Vector<int> dst(1000, 0);

    for (auto _ : state) {
        dst = src;
    }
}
BENCHMARK(CopyAssignment_VectorPro);

static void CopyAssignment_StdVector(benchmark::State& state) {
    std::vector<int> stdSrc(1000, 7);
    std::vector<int> stdDst(1000, 0);

    for (auto _ : state) {
        stdDst = stdSrc;
    }
}
BENCHMARK(CopyAssignment_StdVector);
