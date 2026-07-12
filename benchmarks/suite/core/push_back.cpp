// Vector Push Back Benchmark Suite
// Measures push_back / emplace_back performance against std::vector.
//
// Covers:
// - push_back
// - reserved push_back
// - emplace_back

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures push_back performance.
static void bench_push_back() {
    auto vp = [&] {
        Vector<int> v;
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("VectorPro push_back", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v;
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("std::vector push_back", SMALL, sv);
}

// Measures push_back performance with reserved capacity.
static void bench_push_back_reserved() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("VectorPro push_back reserved", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("std::vector push_back reserved", SMALL, sv);
}

// Measures emplace_back performance.
static void bench_emplace_back() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.emplace_back(i);
        doNotOptimize(v);
    };
    BENCH("VectorPro emplace_back", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.emplace_back(i);
        doNotOptimize(v);
    };
    BENCH("std::vector emplace_back", SMALL, sv);
}

// Executes all push_back benchmark cases.
static void run_benchmarks() {
    bench_push_back();
    std::cout << "\n";

    bench_push_back_reserved();
    std::cout << "\n";

    bench_emplace_back();
}

REGISTER_BENCH_SUITE();
