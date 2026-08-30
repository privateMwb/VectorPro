// Vector Push Back Benchmark Suite
// Measures push_back / emplace_back performance against std::vector.
//
// Covers:
// - push_back
// - reserved push_back
// - emplace_back

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// push_back
// ---------------------------------------------------------------------------

static void PushBack_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v;
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(PushBack_VectorPro);

static void PushBack_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(PushBack_StdVector);

// ---------------------------------------------------------------------------
// push_back reserved
// ---------------------------------------------------------------------------

static void PushBackReserved_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(PushBackReserved_VectorPro);

static void PushBackReserved_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(PushBackReserved_StdVector);

// ---------------------------------------------------------------------------
// emplace_back
// ---------------------------------------------------------------------------

static void EmplaceBack_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.emplace_back(i);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EmplaceBack_VectorPro);

static void EmplaceBack_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.emplace_back(i);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EmplaceBack_StdVector);
