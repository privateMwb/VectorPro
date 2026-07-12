// Vector Erase Benchmark Suite
// Measures erase() performance against std::vector.
//
// Covers:
// - erase (front, middle, end)

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures erasure at the beginning.
static void bench_erase_front() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.erase(v.cbegin());
        doNotOptimize(v);
    };
    BENCH("VectorPro erase front", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.erase(v.cbegin());
        doNotOptimize(v);
    };
    BENCH("std::vector erase front", SMALL, sv);
}

// Measures erasure in the middle.
static void bench_erase_middle() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.erase(v.cbegin() + 250);
        doNotOptimize(v);
    };
    BENCH("VectorPro erase middle", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.erase(v.cbegin() + 250);
        doNotOptimize(v);
    };
    BENCH("std::vector erase middle", SMALL, sv);
}

// Measures erasure at the end.
static void bench_erase_end() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.erase(v.cend() - 1);
        doNotOptimize(v);
    };
    BENCH("VectorPro erase end", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.erase(v.cend() - 1);
        doNotOptimize(v);
    };
    BENCH("std::vector erase end", SMALL, sv);
}

// Executes all erase benchmark cases.
static void run_benchmarks() {
    bench_erase_front();
    std::cout << "\n";

    bench_erase_middle();
    std::cout << "\n";

    bench_erase_end();
}

REGISTER_BENCH_SUITE();
