// Vector iteration benchmark suite.
//
// Measures iteration performance against std::vector.
//
// Coverage:
// - Range-for iteration
// - Iterator traversal
// - Index-based traversal
// - Reverse iteration
// - std::accumulate

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <numeric>
#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// range-for
// ---------------------------------------------------------------------------

static void RangeFor_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (int x : v)
            sum += x;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(RangeFor_VectorPro);

static void RangeFor_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (int x : sv)
            sum += x;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(RangeFor_StdVector);

// ---------------------------------------------------------------------------
// iterator loop
// ---------------------------------------------------------------------------

static void IteratorLoop_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (auto it = v.begin(); it != v.end(); ++it)
            sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(IteratorLoop_VectorPro);

static void IteratorLoop_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (auto it = sv.begin(); it != sv.end(); ++it)
            sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(IteratorLoop_StdVector);

// ---------------------------------------------------------------------------
// index loop
// ---------------------------------------------------------------------------

static void IndexLoop_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (std::size_t i = 0; i < v.size(); ++i)
            sum += v[i];
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(IndexLoop_VectorPro);

static void IndexLoop_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (std::size_t i = 0; i < sv.size(); ++i)
            sum += sv[i];
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(IndexLoop_StdVector);

// ---------------------------------------------------------------------------
// reverse iteration
// ---------------------------------------------------------------------------

static void ReverseIteration_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (auto it = v.rbegin(); it != v.rend(); ++it)
            sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(ReverseIteration_VectorPro);

static void ReverseIteration_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (auto it = sv.rbegin(); it != sv.rend(); ++it)
            sum += *it;
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(ReverseIteration_StdVector);

// ---------------------------------------------------------------------------
// accumulate
// ---------------------------------------------------------------------------

static void Accumulate_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        long sum = std::accumulate(v.begin(), v.end(), 0L);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(Accumulate_VectorPro);

static void Accumulate_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        long sum = std::accumulate(sv.begin(), sv.end(), 0L);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(Accumulate_StdVector);
