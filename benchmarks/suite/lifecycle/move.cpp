// Vector Move Benchmark Suite
// Measures move construction and move assignment performance against std::vector.
//
// Covers:
// - move construction
// - move assignment

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures move construction.
static void bench_move_construct() {
    auto vp = [&] {
        Vector<int> src(100, 7);
        Vector<int> dst(std::move(src));
        doNotOptimize(dst);
    };
    BENCH("VectorPro move construct", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> src(100, 7);
        std::vector<int> dst(std::move(src));
        doNotOptimize(dst);
    };
    BENCH("std::vector move construct", MEDIUM, sv);
}

// Measures move assignment.
static void bench_move_assignment() {
    auto vp = [&] {
        Vector<int> src(100, 7);
        Vector<int> dst;
        dst = std::move(src);
        doNotOptimize(dst);
    };
    BENCH("VectorPro move assignment", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> src(100, 7);
        std::vector<int> dst;
        dst = std::move(src);
        doNotOptimize(dst);
    };
    BENCH("std::vector move assignment", MEDIUM, sv);
}

// Executes all move benchmark cases.
static void run_benchmarks() {
    bench_move_construct();
    std::cout << "\n";

    bench_move_assignment();
}

REGISTER_BENCH_SUITE();
