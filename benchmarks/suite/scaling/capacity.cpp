// Vector Capacity Benchmark Suite
// Measures the performance of capacity-management operations against
// equivalent std::vector operations.
//
// Covers:
// - reserve
// - reserve with repeated growth
// - shrink_to_fit
// - growth-triggered reallocation
// - reserve no-op

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

// Measures shrink_to_fit() after over-reserving capacity.
static void bench_shrink_to_fit() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
        v.shrink_to_fit();
        doNotOptimize(v);
    };
    BENCH("VectorPro shrink_to_fit", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
        v.shrink_to_fit();
        doNotOptimize(v);
    };
    BENCH("std::vector shrink_to_fit", MEDIUM, sv);
}

// Measures the cost of a single growth-triggered reallocation with a
// populated buffer, isolating the move/copy-and-free overhead from
// push_back() itself.
static void bench_growth_reallocation() {
    auto vp = [&] {
        Vector<int> v(1024, 0);
        v.push_back(99); // size == capacity, forces one reallocation
        doNotOptimize(v);
    };
    BENCH("VectorPro growth reallocation", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> v(1024, 0);
        v.push_back(99);
        doNotOptimize(v);
    };
    BENCH("std::vector growth reallocation", MEDIUM, sv);
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

// Executes all capacity benchmark cases.
static void run_benchmarks() {
    bench_reserve();
    std::cout << "\n";

    bench_reserve_growth();
    std::cout << "\n";

    bench_shrink_to_fit();
    std::cout << "\n";

    bench_growth_reallocation();
    std::cout << "\n";

    bench_reserve_noop();
}

REGISTER_BENCH_SUITE();