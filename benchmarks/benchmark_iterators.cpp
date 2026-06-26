// Vector Iterator Benchmark
// Measures forward, reverse, and const iterator performance
// across traversal, arithmetic, and algorithm operations
// against std::vector where applicable.
//
// Covers:
// - forward traversal
// - const forward traversal
// - reverse traversal
// - iterator arithmetic
// - std::sort
// - std::find
// - std::accumulate
// - range-based for loop

#include "benchmark_helper.h"

#include <vector>
#include <algorithm>
#include <numeric>

using namespace VectorPro;

static void forward_traversal() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    auto fn1 = [&](){
        int sum = 0;
        for (auto it = v.begin(); it != v.end(); ++it)
            sum += *it;
        (void)sum;
    };
    auto fn2 = [&](){
        int sum = 0;
        for (auto it = stdV.begin(); it != stdV.end(); ++it)
            sum += *it;
        (void)sum;
    };
    BENCH("VectorPro forward traversal",   ITERATIONS, fn1());
    BENCH("std::vector forward traversal", ITERATIONS, fn2());
}

static void const_forward_traversal() {
    const Vector<int>      v    = { 1, 2, 3, 4, 5 };
    const std::vector<int> stdV = { 1, 2, 3, 4, 5 };

    auto fn1 = [&](){
        int sum = 0;
        for (auto it = v.cbegin(); it != v.cend(); ++it)
            sum += *it;
        (void)sum;
    };
    auto fn2 = [&](){
        int sum = 0;
        for (auto it = stdV.cbegin(); it != stdV.cend(); ++it)
            sum += *it;
        (void)sum;
    };
    BENCH("VectorPro const forward traversal",   ITERATIONS, fn1());
    BENCH("std::vector const forward traversal", ITERATIONS, fn2());
}

static void reverse_traversal() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    auto fn1 = [&](){
        int sum = 0;
        for (auto it = v.rbegin(); it != v.rend(); ++it)
            sum += *it;
        (void)sum;
    };
    auto fn2 = [&](){
        int sum = 0;
        for (auto it = stdV.rbegin(); it != stdV.rend(); ++it)
            sum += *it;
        (void)sum;
    };
    BENCH("VectorPro reverse traversal",   ITERATIONS, fn1());
    BENCH("std::vector reverse traversal", ITERATIONS, fn2());
}

static void iterator_arithmetic() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    BENCH("VectorPro iterator +",   ITERATIONS, (void)(v.begin()    + 50));
    BENCH("std::vector iterator +", ITERATIONS, (void)(stdV.begin() + 50));
    BENCH("VectorPro iterator -",   ITERATIONS, (void)(v.end()      - 50));
    BENCH("std::vector iterator -", ITERATIONS, (void)(stdV.end()   - 50));
}

static void std_sort() {
    auto fn1 = [](){
        Vector<int> v = { 5, 3, 1, 4, 2, 9, 7, 6, 8, 0 };
        std::sort(v.begin(), v.end());
    };
    auto fn2 = [](){
        std::vector<int> v = { 5, 3, 1, 4, 2, 9, 7, 6, 8, 0 };
        std::sort(v.begin(), v.end());
    };
    BENCH("VectorPro std::sort",   ITERATIONS, fn1());
    BENCH("std::vector std::sort", ITERATIONS, fn2());
}

static void std_find() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    BENCH("VectorPro std::find hit",    ITERATIONS, (void)std::find(v.begin(),    v.end(),    0));
    BENCH("std::vector std::find hit",  ITERATIONS, (void)std::find(stdV.begin(), stdV.end(), 0));
    BENCH("VectorPro std::find miss",   ITERATIONS, (void)std::find(v.begin(),    v.end(),    -1));
    BENCH("std::vector std::find miss", ITERATIONS, (void)std::find(stdV.begin(), stdV.end(), -1));
}

static void std_accumulate() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    BENCH("VectorPro std::accumulate",   ITERATIONS, (void)std::accumulate(v.begin(),    v.end(),    0));
    BENCH("std::vector std::accumulate", ITERATIONS, (void)std::accumulate(stdV.begin(), stdV.end(), 0));
}

static void range_based_for() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    auto fn1 = [&](){
        int sum = 0;
        for (const auto& x : v)
            sum += x;
        (void)sum;
    };
    auto fn2 = [&](){
        int sum = 0;
        for (const auto& x : stdV)
            sum += x;
        (void)sum;
    };
    BENCH("VectorPro range-based for",   ITERATIONS, fn1());
    BENCH("std::vector range-based for", ITERATIONS, fn2());
}

void run_iterator_benchmarks() {
    bench_header("Iterator");

    forward_traversal();
    std::cout << "\n";

    const_forward_traversal();
    std::cout << "\n";

    reverse_traversal();
    std::cout << "\n";

    iterator_arithmetic();
    std::cout << "\n";

    std_sort();
    std::cout << "\n";

    std_find();
    std::cout << "\n";

    std_accumulate();
    std::cout << "\n";

    range_based_for();

    bench_footer();
}