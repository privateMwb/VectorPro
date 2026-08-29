// Vector Search Benchmark Suite
// Measures search performance against equivalent std::vector operations.
//
// Covers:
// - contains() hit
// - contains() miss
// - contains() hit on a large vector
// - find() hit
// - find() miss

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <algorithm>
#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// contains hit (1k)
// ---------------------------------------------------------------------------

static void ContainsHitSmall_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        bool r = v.contains(500);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(ContainsHitSmall_VectorPro);

static void ContainsHitSmall_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        bool r = std::find(sv.begin(), sv.end(), 500) != sv.end();
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(ContainsHitSmall_StdVector);

// ---------------------------------------------------------------------------
// contains miss (1k)
// ---------------------------------------------------------------------------

static void ContainsMissSmall_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        bool r = v.contains(-1);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(ContainsMissSmall_VectorPro);

static void ContainsMissSmall_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        bool r = std::find(sv.begin(), sv.end(), -1) != sv.end();
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(ContainsMissSmall_StdVector);

// ---------------------------------------------------------------------------
// contains hit (10k)
// ---------------------------------------------------------------------------

static void ContainsHitLarge_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(10'000);
    for (int i = 0; i < 10'000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        bool r = v.contains(10'000);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(ContainsHitLarge_VectorPro);

static void ContainsHitLarge_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(10'000);
    for (int i = 0; i < 10'000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        bool r = std::find(sv.begin(), sv.end(), 10'000) != sv.end();
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(ContainsHitLarge_StdVector);

// ---------------------------------------------------------------------------
// find hit (1k)
// ---------------------------------------------------------------------------

static void FindHitSmall_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        auto it = v.find(500);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(FindHitSmall_VectorPro);

static void FindHitSmall_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        auto it = std::find(sv.begin(), sv.end(), 500);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(FindHitSmall_StdVector);

// ---------------------------------------------------------------------------
// find miss (1k)
// ---------------------------------------------------------------------------

static void FindMissSmall_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        auto it = v.find(-1);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(FindMissSmall_VectorPro);

static void FindMissSmall_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        auto it = std::find(sv.begin(), sv.end(), -1);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(FindMissSmall_StdVector);
