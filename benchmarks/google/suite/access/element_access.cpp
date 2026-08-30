// Vector Element Access Benchmark Suite
// Measures single-element accessor performance against std::vector.
//
// Covers:
// - at() bounds-checked access
// - front() / back()
// - raw pointer access via data_ptr() / data()

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// at()
// ---------------------------------------------------------------------------

static void At_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (std::size_t i = 0; i < v.size(); ++i)
            sum += v.at(i);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(At_VectorPro);

static void At_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        for (std::size_t i = 0; i < sv.size(); ++i)
            sum += sv.at(i);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(At_StdVector);

// ---------------------------------------------------------------------------
// front/back
// ---------------------------------------------------------------------------

static void FrontBack_VectorPro(benchmark::State& state) {
    Vector<int> v(1000, 7);

    for (auto _ : state) {
        long sum = 0;
        for (int i = 0; i < 1000; ++i)
            sum += v.front() + v.back();
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(FrontBack_VectorPro);

static void FrontBack_StdVector(benchmark::State& state) {
    std::vector<int> sv(1000, 7);

    for (auto _ : state) {
        long sum = 0;
        for (int i = 0; i < 1000; ++i)
            sum += sv.front() + sv.back();
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(FrontBack_StdVector);

// ---------------------------------------------------------------------------
// data_ptr() / data()
// ---------------------------------------------------------------------------

static void DataPtr_VectorPro(benchmark::State& state) {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        const int* p = v.data_ptr();
        for (std::size_t i = 0; i < v.size(); ++i)
            sum += p[i];
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(DataPtr_VectorPro);

static void DataPtr_StdVector(benchmark::State& state) {
    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    for (auto _ : state) {
        long sum = 0;
        const int* p = sv.data();
        for (std::size_t i = 0; i < sv.size(); ++i)
            sum += p[i];
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(DataPtr_StdVector);
