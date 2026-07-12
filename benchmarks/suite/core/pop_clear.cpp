// Vector Pop Clear Benchmark Suite
// Measures pop_back() and clear() performance against std::vector.
//
// Covers:
// - pop_back
// - clear

#include <common/framework.h>

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
    BENCH("VectorPro pop_back", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(1000, 0);
        while (!v.empty())
            v.pop_back();
        doNotOptimize(v);
    };
    BENCH("std::vector pop_back", SMALL, sv);
}

// Measures clear performance.
static void bench_clear() {
    auto vp = [&] {
        Vector<int> v(1000, 0);
        v.clear();
        doNotOptimize(v);
    };
    BENCH("VectorPro clear", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(1000, 0);
        v.clear();
        doNotOptimize(v);
    };
    BENCH("std::vector clear", SMALL, sv);
}

// Executes all pop_back/clear benchmark cases.
static void run_benchmarks() {
    bench_pop_back();
    std::cout << "\n";

    bench_clear();
}

REGISTER_BENCH_SUITE();
