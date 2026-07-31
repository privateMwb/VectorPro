// Vector Insert Benchmark Suite
// Measures insert() performance against std::vector.
//
// Covers:
// - insert (front, middle, end)

#include <support/framework.h>

#include <vector>

using namespace VectorPro;

// Measures insertion at the beginning.
static void bench_insert_front() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.insert(v.cbegin(), 99);
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.insert(v.cbegin(), 99);
        doNotOptimize(v);
    };

    BENCH("insert front", vp, sv);
}

// Measures insertion in the middle.
static void bench_insert_middle() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.insert(v.cbegin() + 250, 99);
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.insert(v.cbegin() + 250, 99);
        doNotOptimize(v);
    };

    BENCH("insert middle", vp, sv);
}

// Measures insertion at the end.
static void bench_insert_end() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.insert(v.cend(), 99);
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.insert(v.cend(), 99);
        doNotOptimize(v);
    };

    BENCH("insert end", vp, sv);
}

// Executes all insert benchmark cases.
static void run_benchmarks() {
    bench_insert_front();
    std::cout << "\n";

    bench_insert_middle();
    std::cout << "\n";

    bench_insert_end();
}

REGISTER_BENCH_SUITE();
