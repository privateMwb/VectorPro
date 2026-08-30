// Vector Pop Clear Benchmark Suite
// Measures pop_back() and clear() performance against std::vector.
//
// Covers:
// - pop_back
// - clear

#include <support/framework.h>

#include <vector>

using namespace VectorPro;

// Measures pop_back performance.
static void bench_pop_back() {
    auto vp = [&] {
        Vector<int> v(1000, 0);
        while (!v.empty())
            v.pop_back();
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(1000, 0);
        while (!v.empty())
            v.pop_back();
        doNotOptimize(v);
    };

    BENCH("pop_back", vp, sv);
}

// Measures clear performance.
static void bench_clear() {
    auto vp = [&] {
        Vector<int> v(1000, 0);
        v.clear();
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(1000, 0);
        v.clear();
        doNotOptimize(v);
    };

    BENCH("clear", vp, sv);
}

// Executes all pop_back/clear benchmark cases.
static void run_benchmarks() {
    bench_pop_back();
    std::cout << "\n";

    bench_clear();
}

REGISTER_BENCH_SUITE();
