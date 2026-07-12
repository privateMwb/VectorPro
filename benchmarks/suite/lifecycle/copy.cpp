// Vector Copy Benchmark Suite
// Measures copy construction and copy assignment performance against std::vector.
//
// Covers:
// - copy construction
// - copy assignment

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures copy construction from a populated vector.
static void bench_copy_construct() {
    Vector<int> src(1000, 7);
    std::vector<int> stdSrc(1000, 7);

    auto vp = [&] {
        Vector<int> c(src);
        doNotOptimize(c);
    };
    BENCH("VectorPro copy construct", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> c(stdSrc);
        doNotOptimize(c);
    };
    BENCH("std::vector copy construct", MEDIUM, sv);
}

// Measures copy assignment with reusable destination capacity.
static void bench_copy_assignment() {
    Vector<int> src(1000, 7);
    std::vector<int> stdSrc(1000, 7);

    Vector<int> dst(1000, 0);
    std::vector<int> stdDst(1000, 0);

    auto vp = [&] { dst = src; };
    BENCH("VectorPro copy assignment", MEDIUM, vp);

    auto sv = [&] { stdDst = stdSrc; };
    BENCH("std::vector copy assignment", MEDIUM, sv);
}

// Executes all copy benchmark cases.
static void run_benchmarks() {
    bench_copy_construct();
    std::cout << "\n";

    bench_copy_assignment();
}

REGISTER_BENCH_SUITE();
