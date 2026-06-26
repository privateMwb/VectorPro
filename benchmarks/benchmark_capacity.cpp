// Vector Capacity Benchmark
// Measures size, capacity, and empty state query performance
// as well as growth behavior against std::vector.
//
// Covers:
// - empty()
// - size()
// - capacity()
// - reserve()
// - shrink_to_fit()
// - growth with default factor
// - growth with custom factor

#include "benchmark_helper.h"

#include <vector>

using namespace VectorPro;

static void empty() {
    Vector<int>      v;
    std::vector<int> stdV;

    BENCH("VectorPro empty()",   ITERATIONS, (void)v.empty());
    BENCH("std::vector empty()", ITERATIONS, (void)stdV.empty());
}

static void size() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    BENCH("VectorPro size()",   ITERATIONS, (void)v.size());
    BENCH("std::vector size()", ITERATIONS, (void)stdV.size());
}

static void capacity() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    BENCH("VectorPro capacity()",   ITERATIONS, (void)v.capacity());
    BENCH("std::vector capacity()", ITERATIONS, (void)stdV.capacity());
}

static void reserve() {
    auto fn1 = [](){
        Vector<int> v;
        v.reserve(100);
    };
    auto fn2 = [](){
        std::vector<int> v;
        v.reserve(100);
    };
    BENCH("VectorPro reserve()",   ITERATIONS, fn1());
    BENCH("std::vector reserve()", ITERATIONS, fn2());
}

static void shrink_to_fit() {
    Vector<int>      v;
    std::vector<int> stdV;

    v.push_back(1);
    stdV.push_back(1);

    BENCH("VectorPro shrink_to_fit()",   ITERATIONS, (v.reserve(100),    v.shrink_to_fit()));
    BENCH("std::vector shrink_to_fit()", ITERATIONS, (stdV.reserve(100), stdV.shrink_to_fit()));
}

static void growth_default() {
    auto fn1 = [](){
        Vector<int> v;
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
    };
    auto fn2 = [](){
        std::vector<int> v;
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
    };
    BENCH("VectorPro default growth (2x)",   ITERATIONS, fn1());
    BENCH("std::vector default growth",      ITERATIONS, fn2());
}

static void growth_custom() {
    auto fn1 = [](){
        Vector<int, std::allocator<int>, 3, 2> v;
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
    };
    auto fn2 = [](){
        Vector<int, std::allocator<int>, 4, 3> v;
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
    };
    BENCH("VectorPro 1.5x growth (3/2)", ITERATIONS, fn1());
    BENCH("VectorPro 1.3x growth (4/3)", ITERATIONS, fn2());
}

void run_capacity_benchmarks() {
    bench_header("Capacity");

    empty();
    std::cout << "\n";

    size();
    std::cout << "\n";

    capacity();
    std::cout << "\n";

    reserve();
    std::cout << "\n";

    shrink_to_fit();
    std::cout << "\n";

    growth_default();
    std::cout << "\n";

    growth_custom();

    bench_footer();
}