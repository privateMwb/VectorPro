// Vector Constructor Benchmark Suite
// Measures construction and assignment performance against std::vector.
//
// Covers:
// - default construction
// - fill construction
// - initializer list construction
// - copy construction
// - move construction
// - copy assignment
// - move assignment

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
    BENCH("VectorPro fill construct", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> v(100, 0);
        doNotOptimize(v);
    };
    BENCH("std::vector fill construct", MEDIUM, sv);
}

// Measures initializer list construction.
static void bench_init_list_construct() {
    auto vp = [&] {
        Vector<int> v{1, 2, 3, 4, 5};
        doNotOptimize(v);
    };
    BENCH("VectorPro init list construct", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> v{1, 2, 3, 4, 5};
        doNotOptimize(v);
    };
    BENCH("std::vector init list construct", MEDIUM, sv);
}

// Measures copy construction from a populated vector.
static void bench_copy_construct() {
    Vector<int> src(1000, 7);
    std::vector<int> stdSrc(1000, 7);

    auto vp = [&] {
        Vector<int> c(src);
        doNotOptimize(c);
    };
    BENCH("VectorPro copy construct", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> c(stdSrc);
        doNotOptimize(c);
    };
    BENCH("std::vector copy construct", MEDIUM, sv);
}

// Measures move construction.
static void bench_move_construct() {
    auto vp = [&] {
        Vector<int> src(100, 7);
        Vector<int> dst(std::move(src));
        doNotOptimize(dst);
    };
    BENCH("VectorPro move construct", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> src(100, 7);
        std::vector<int> dst(std::move(src));
        doNotOptimize(dst);
    };
    BENCH("std::vector move construct", MEDIUM, sv);
}

// Measures copy assignment with reusable destination capacity.
static void bench_copy_assignment() {
    Vector<int> src(1000, 7);
    std::vector<int> stdSrc(1000, 7);

    Vector<int> dst(1000, 0);
    std::vector<int> stdDst(1000, 0);

    auto vp = [&] { dst = src; };
    BENCH("VectorPro copy assignment", MEDIUM, vp);

    auto sv = [&] { stdDst = stdSrc; };
    BENCH("std::vector copy assignment", MEDIUM, sv);
}

// Measures move assignment.
static void bench_move_assignment() {
    auto vp = [&] {
        Vector<int> src(100, 7);
        Vector<int> dst;
        dst = std::move(src);
        doNotOptimize(dst);
    };
    BENCH("VectorPro move assignment", MEDIUM, vp);

    auto sv = [&] {
        std::vector<int> src(100, 7);
        std::vector<int> dst;
        dst = std::move(src);
        doNotOptimize(dst);
    };
    BENCH("std::vector move assignment", MEDIUM, sv);
}

// Executes all constructor benchmark cases.
static void run_benchmarks() {
    bench_default_construct();
    std::cout << "\n";

    bench_fill_construct();
    std::cout << "\n";

    bench_init_list_construct();
    std::cout << "\n";

    bench_copy_construct();
    std::cout << "\n";

    bench_move_construct();
    std::cout << "\n";

    bench_copy_assignment();
    std::cout << "\n";

    bench_move_assignment();
}

REGISTER_BENCH_SUITE();