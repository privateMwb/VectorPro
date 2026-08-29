// Vector Reallocation Benchmark Suite
// Measures the cost of a single growth-triggered reallocation, isolating
// the move/copy-and-free overhead from push_back() itself.
//
// Covers:
// - growth-triggered reallocation with a populated buffer

#include <support/framework.h>

#include <vector>

using namespace VectorPro;

// Measures a single reallocation forced by size == capacity.
static void bench_growth_reallocation() {
    auto vp = [&] {
        Vector<int> v(1024, 0);
        v.push_back(99); // size == capacity, forces one reallocation
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(1024, 0);
        v.push_back(99);
        doNotOptimize(v);
    };

    BENCH("growth reallocation", vp, sv);
}

// Executes all reallocation benchmark cases.
static void run_benchmarks() {
    bench_growth_reallocation();
}

REGISTER_BENCH_SUITE();
