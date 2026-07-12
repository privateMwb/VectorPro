// Vector Reserve Benchmark Suite
// Measures the performance of reserve() against equivalent std::vector
// operations.
//
// Covers:
// - reserve on an empty vector
// - reserve with repeated growth targets
// - reserve no-op (capacity already sufficient)

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures reserve() on an empty vector.
static void bench_reserve() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        doNotOptimize(v);
    };
    BENCH("VectorPro reserve", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        doNotOptimize(v);
    };
    BENCH("std::vector reserve", MEDIUM, sv);
}

// Measures repeated reserve() calls with increasing capacity targets.
static void bench_reserve_growth() {
    auto vp = [&] {
        Vector<int> v;
        for (std::size_t cap = 64; cap <= 4096; cap *= 2)
            v.reserve(cap);
        doNotOptimize(v);
    };
    BENCH("VectorPro reserve growth", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v;
        for (std::size_t cap = 64; cap <= 4096; cap *= 2)
            v.reserve(cap);
        doNotOptimize(v);
    };
    BENCH("std::vector reserve growth", SMALL, sv);
}

// Measures reserve() when the requested capacity is already available.
static void bench_reserve_noop() {
    Vector<int> vp_v;
    vp_v.reserve(2000);

    std::vector<int> sv_v;
    sv_v.reserve(2000);

    auto vp = [&] {
        vp_v.reserve(1000);
        doNotOptimize(vp_v);
    };
    BENCH("VectorPro reserve no-op", LARGE, vp);

    auto sv = [&] {
        sv_v.reserve(1000);
        doNotOptimize(sv_v);
    };
    BENCH("std::vector reserve no-op", LARGE, sv);
}

// Executes all reserve benchmark cases.
static void run_benchmarks() {
    bench_reserve();
    std::cout << "\n";

    bench_reserve_growth();
    std::cout << "\n";

    bench_reserve_noop();
}

REGISTER_BENCH_SUITE();
