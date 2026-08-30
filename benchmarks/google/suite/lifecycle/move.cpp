// Vector Move Benchmark Suite
// Measures move construction and move assignment performance against std::vector.
//
// Covers:
// - move construction
// - move assignment

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// move construct
// ---------------------------------------------------------------------------

static void MoveConstruct_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> src(100, 7);
        Vector<int> dst(std::move(src));
        benchmark::DoNotOptimize(dst);
    }
}
BENCHMARK(MoveConstruct_VectorPro);

static void MoveConstruct_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> src(100, 7);
        std::vector<int> dst(std::move(src));
        benchmark::DoNotOptimize(dst);
    }
}
BENCHMARK(MoveConstruct_StdVector);

// ---------------------------------------------------------------------------
// move assignment
// ---------------------------------------------------------------------------

static void MoveAssignment_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> src(100, 7);
        Vector<int> dst;
        dst = std::move(src);
        benchmark::DoNotOptimize(dst);
    }
}
BENCHMARK(MoveAssignment_VectorPro);

static void MoveAssignment_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> src(100, 7);
        std::vector<int> dst;
        dst = std::move(src);
        benchmark::DoNotOptimize(dst);
    }
}
BENCHMARK(MoveAssignment_StdVector);
