// Vector Remove If Benchmark Suite
// Measures remove_if() against std::vector's erase-remove idiom.
//
// Covers:
// - remove_if

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <algorithm>
#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// erase-remove_if
// ---------------------------------------------------------------------------

static void EraseRemoveIf_VectorPro(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        (void)v.remove_if([](const int& x) { return x % 2 == 0; });
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EraseRemoveIf_VectorPro);

static void EraseRemoveIf_StdVector(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        v.erase(std::remove_if(v.begin(), v.end(), [](const int& x) { return x % 2 == 0; }),
                v.end());
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(EraseRemoveIf_StdVector);
