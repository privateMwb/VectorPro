// Vector Construction Benchmark Suite
// Measures construction performance against std::vector.
//
// Covers:
// - default construction
// - fill construction
// - initializer list construction

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures default construction.
static void bench_default_construct() {
    auto vp = [&] {
        Vector<int> v;
        doNotOptimize(v);
    };
    BENCH("VectorPro default construct", LARGE, vp);

    auto sv = [&] {
        std::vector<int> v;
        doNotOptimize(v);
    };
    BENCH("std::vector default construct", LARGE, sv);
}

// Measures fill construction of 100 elements.
static void bench_fill_construct() {
    auto vp = [&] {
        Vector<int> v(100, 0);
        doNotOptimize(v);
    };
    BENCH("VectorPro fill construct", LARGE, vp);

    auto sv = [&] {
        std::vector<int> v(100, 0);
        doNotOptimize(v);
    };
    BENCH("std::vector fill construct", LARGE, sv);
}

// Measures initializer list construction.
static void bench_init_list_construct() {
    auto vp = [&] {
        Vector<int> v{1, 2, 3, 4, 5};
        doNotOptimize(v);
    };
    BENCH("VectorPro init list construct", LARGE, vp);

    auto sv = [&] {
        std::vector<int> v{1, 2, 3, 4, 5};
        doNotOptimize(v);
    };
    BENCH("std::vector init list construct", LARGE, sv);
}

// Executes all construction benchmark cases.
static void run_benchmarks() {
    bench_default_construct();
    std::cout << "\n";

    bench_fill_construct();
    std::cout << "\n";

    bench_init_list_construct();
}

REGISTER_BENCH_SUITE();
