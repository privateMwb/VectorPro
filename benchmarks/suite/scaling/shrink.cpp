// Vector Shrink Benchmark Suite
// Measures the performance of shrink_to_fit() against equivalent
// std::vector operations.
//
// Covers:
// - shrink_to_fit after over-reserving capacity

#include <support/framework.h>

#include <vector>

using namespace VectorPro;

// Measures shrink_to_fit() after over-reserving capacity.
static void bench_shrink_to_fit() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
        v.shrink_to_fit();
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
        v.shrink_to_fit();
        doNotOptimize(v);
    };

    BENCH("shrink_to_fit", vp, sv);
}

// Executes all shrink benchmark cases.
static void run_benchmarks() {
    bench_shrink_to_fit();
}

REGISTER_BENCH_SUITE();
