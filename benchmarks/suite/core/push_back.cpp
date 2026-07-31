// Vector Push Back Benchmark Suite
// Measures push_back / emplace_back performance against std::vector.
//
// Covers:
// - push_back
// - reserved push_back
// - emplace_back

#include <support/framework.h>

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

    auto sv = [&] {
        std::vector<int> v;
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };

    BENCH("push_back", vp, sv);
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

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };

    BENCH("push_back reserved", vp, sv);
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

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.emplace_back(i);
        doNotOptimize(v);
    };

    BENCH("emplace_back", vp, sv);
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
