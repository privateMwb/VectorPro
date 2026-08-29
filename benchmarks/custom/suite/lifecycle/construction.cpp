// Vector Construction Benchmark Suite
// Measures construction performance against std::vector.
//
// Covers:
// - default construction
// - fill construction
// - initializer list construction

#include <support/framework.h>

#include <vector>

using namespace VectorPro;

// Measures default construction.
static void bench_default_construct() {
    auto vp = [&] {
        Vector<int> v;
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v;
        doNotOptimize(v);
    };

    BENCH("default construct", vp, sv);
}

// Measures fill construction of 100 elements.
static void bench_fill_construct() {
    auto vp = [&] {
        Vector<int> v(100, 0);
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v(100, 0);
        doNotOptimize(v);
    };

    BENCH("fill construct", vp, sv);
}

// Measures initializer list construction.
static void bench_init_list_construct() {
    auto vp = [&] {
        Vector<int> v{1, 2, 3, 4, 5};
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v{1, 2, 3, 4, 5};
        doNotOptimize(v);
    };

    BENCH("init list construct", vp, sv);
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
