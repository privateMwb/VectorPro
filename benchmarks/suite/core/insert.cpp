// Vector Insert Benchmark Suite
// Measures insert() performance against std::vector.
//
// Covers:
// - insert (front, middle, end)

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures insertion at the beginning.
static void bench_insert_front() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.insert(v.cbegin(), 99);
        doNotOptimize(v);
    };
    BENCH("VectorPro insert front", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.insert(v.cbegin(), 99);
        doNotOptimize(v);
    };
    BENCH("std::vector insert front", SMALL, sv);
}

// Measures insertion in the middle.
static void bench_insert_middle() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.insert(v.cbegin() + 250, 99);
        doNotOptimize(v);
    };
    BENCH("VectorPro insert middle", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.insert(v.cbegin() + 250, 99);
        doNotOptimize(v);
    };
    BENCH("std::vector insert middle", SMALL, sv);
}

// Measures insertion at the end.
static void bench_insert_end() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.insert(v.cend(), 99);
        doNotOptimize(v);
    };
    BENCH("VectorPro insert end", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.insert(v.cend(), 99);
        doNotOptimize(v);
    };
    BENCH("std::vector insert end", SMALL, sv);
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
