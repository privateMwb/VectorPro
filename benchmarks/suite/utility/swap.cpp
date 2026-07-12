// Vector swap benchmark suite.
//
// Measures swap performance against std::vector.
//
// Coverage:
// - member swap()
// - free-function swap() (ADL)

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures member swap() between two populated vectors.
static void bench_member_swap() {
    auto vp = [&] {
        Vector<int> a(1000, 1);
        Vector<int> b(1000, 2);
        a.swap(b);
        doNotOptimize(a);
        doNotOptimize(b);
    };
    BENCH("VectorPro member swap", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> a(1000, 1);
        std::vector<int> b(1000, 2);
        a.swap(b);
        doNotOptimize(a);
        doNotOptimize(b);
    };
    BENCH("std::vector member swap", MEDIUM, sv);
}

// Measures free-function swap() resolved via ADL.
static void bench_free_swap() {
    auto vp = [&] {
        Vector<int> a(1000, 1);
        Vector<int> b(1000, 2);
        swap(a, b);
        doNotOptimize(a);
        doNotOptimize(b);
    };
    BENCH("VectorPro free swap", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> a(1000, 1);
        std::vector<int> b(1000, 2);
        swap(a, b);
        doNotOptimize(a);
        doNotOptimize(b);
    };
    BENCH("std::vector free swap", MEDIUM, sv);
}

// Executes all swap benchmark cases.
static void run_benchmarks() {
    bench_member_swap();
    std::cout << "\n";

    bench_free_swap();
}

REGISTER_BENCH_SUITE();
