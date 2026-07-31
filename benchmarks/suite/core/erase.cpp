// Vector Erase Benchmark Suite
// Measures erase() performance against std::vector.
//
// Covers:
// - erase (front, middle, end)

#include <support/framework.h>

#include <vector>

using namespace VectorPro;

// Measures erasure at the beginning.
static void bench_erase_front() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.erase(v.cbegin());
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.erase(v.cbegin());
        doNotOptimize(v);
    };

    BENCH("erase front", vp, sv);
}

// Measures erasure in the middle.
static void bench_erase_middle() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.erase(v.cbegin() + 250);
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.erase(v.cbegin() + 250);
        doNotOptimize(v);
    };

    BENCH("erase middle", vp, sv);
}

// Measures erasure at the end.
static void bench_erase_end() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.erase(v.cend() - 1);
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.erase(v.cend() - 1);
        doNotOptimize(v);
    };

    BENCH("erase end", vp, sv);
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
