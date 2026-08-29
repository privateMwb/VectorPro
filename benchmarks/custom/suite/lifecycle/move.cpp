// Vector Move Benchmark Suite
// Measures move construction and move assignment performance against std::vector.
//
// Covers:
// - move construction
// - move assignment

#include <support/framework.h>

#include <vector>

using namespace VectorPro;

// Measures move construction.
static void bench_move_construct() {
    auto vp = [&] {
        Vector<int> src(100, 7);
        Vector<int> dst(std::move(src));
        doNotOptimize(dst);
    };

    auto sv = [&] {
        std::vector<int> src(100, 7);
        std::vector<int> dst(std::move(src));
        doNotOptimize(dst);
    };

    BENCH("move construct", vp, sv);
}

// Measures move assignment.
static void bench_move_assignment() {
    auto vp = [&] {
        Vector<int> src(100, 7);
        Vector<int> dst;
        dst = std::move(src);
        doNotOptimize(dst);
    };

    auto sv = [&] {
        std::vector<int> src(100, 7);
        std::vector<int> dst;
        dst = std::move(src);
        doNotOptimize(dst);
    };
    BENCH("move assignment", vp, sv);
}

// Executes all move benchmark cases.
static void run_benchmarks() {
    bench_move_construct();
    std::cout << "\n";

    bench_move_assignment();
}

REGISTER_BENCH_SUITE();
