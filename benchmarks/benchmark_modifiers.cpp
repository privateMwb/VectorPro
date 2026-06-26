// Vector Modifier Benchmark
// Measures insertion, removal, and capacity management
// performance against std::vector where applicable.
//
// Covers:
// - push_back (copy and move)
// - emplace_back
// - insert (single and range)
// - emplace
// - remove_if
// - pop_back
// - erase (single and range)
// - clear
// - reserve
// - shrink_to_fit

#include "benchmark_helper.h"

#include <vector>
#include <algorithm>

using namespace VectorPro;

static void push_back_copy() {
    auto fn1 = [](){
        Vector<int> v;
        int val = 42;
        v.push_back(val);
    };
    auto fn2 = [](){
        std::vector<int> v;
        int val = 42;
        v.push_back(val);
    };
    BENCH("VectorPro push_back copy",   ITERATIONS, fn1());
    BENCH("std::vector push_back copy", ITERATIONS, fn2());
}

static void push_back_move() {
    auto fn1 = [](){
        Vector<int> v;
        v.push_back(42);
    };
    auto fn2 = [](){
        std::vector<int> v;
        v.push_back(42);
    };
    BENCH("VectorPro push_back move",   ITERATIONS, fn1());
    BENCH("std::vector push_back move", ITERATIONS, fn2());
}

static void emplace_back() {
    auto fn1 = [](){
        Vector<std::string> v;
        v.emplace_back(3, 'x');
    };
    auto fn2 = [](){
        std::vector<std::string> v;
        v.emplace_back(3, 'x');
    };
    BENCH("VectorPro emplace_back",   ITERATIONS, fn1());
    BENCH("std::vector emplace_back", ITERATIONS, fn2());
}

static void insert_single() {
    auto fn1 = [](){
        Vector<int> v;
        fillVector(v, 100);
        int val = 99;
        (void)v.insert(v.cbegin(), val);
    };
    auto fn2 = [](){
        std::vector<int> v;
        fillStd(v, 100);
        int val = 99;
        v.insert(v.cbegin(), val);
    };
    BENCH("VectorPro insert single",   ITERATIONS, fn1());
    BENCH("std::vector insert single", ITERATIONS, fn2());
}

static void insert_range() {
    auto fn1 = [](){
        Vector<int> v;
        Vector<int> src = { 1, 2, 3, 4, 5 };
        fillVector(v, 100);
        v.insert(v.cbegin(), src.begin(), src.end());
    };
    auto fn2 = [](){
        std::vector<int> v;
        std::vector<int> src = { 1, 2, 3, 4, 5 };
        fillStd(v, 100);
        v.insert(v.cbegin(), src.begin(), src.end());
    };
    BENCH("VectorPro insert range",   ITERATIONS, fn1());
    BENCH("std::vector insert range", ITERATIONS, fn2());
}

static void emplace() {
    auto fn1 = [](){
        Vector<std::string> v;
        v.push_back("a");
        v.push_back("c");
        (void)v.emplace(v.cbegin() + 1, 1, 'b');
    };
    auto fn2 = [](){
        std::vector<std::string> v;
        v.push_back("a");
        v.push_back("c");
        v.emplace(v.cbegin() + 1, 1, 'b');
    };
    BENCH("VectorPro emplace",   ITERATIONS, fn1());
    BENCH("std::vector emplace", ITERATIONS, fn2());
}

static void remove_if() {
    auto fn1 = [](){
        Vector<int> v;
        fillVector(v, 100);
        (void)v.remove_if([](const int& x){ return x % 2 == 0; });
    };
    auto fn2 = [](){
        std::vector<int> v;
        fillStd(v, 100);
        v.erase(std::remove_if(v.begin(), v.end(),
            [](const int& x){ return x % 2 == 0; }), v.end());
    };
    BENCH("VectorPro remove_if",  ITERATIONS, fn1());
    BENCH("std::vector erase_if", ITERATIONS, fn2());
}

static void pop_back() {
    auto fn1 = [](){
        Vector<int> v;
        fillVector(v, 100);
        v.pop_back();
    };
    auto fn2 = [](){
        std::vector<int> v;
        fillStd(v, 100);
        v.pop_back();
    };
    BENCH("VectorPro pop_back",   ITERATIONS, fn1());
    BENCH("std::vector pop_back", ITERATIONS, fn2());
}

static void erase_single() {
    auto fn1 = [](){
        Vector<int> v;
        fillVector(v, 100);
        (void)v.erase(v.cbegin());
    };
    auto fn2 = [](){
        std::vector<int> v;
        fillStd(v, 100);
        v.erase(v.cbegin());
    };
    BENCH("VectorPro erase single",   ITERATIONS, fn1());
    BENCH("std::vector erase single", ITERATIONS, fn2());
}

static void erase_range() {
    auto fn1 = [](){
        Vector<int> v;
        fillVector(v, 100);
        (void)v.erase(v.cbegin(), v.cbegin() + 10);
    };
    auto fn2 = [](){
        std::vector<int> v;
        fillStd(v, 100);
        v.erase(v.cbegin(), v.cbegin() + 10);
    };
    BENCH("VectorPro erase range",   ITERATIONS, fn1());
    BENCH("std::vector erase range", ITERATIONS, fn2());
}

static void clear() {
    auto fn1 = [](){
        Vector<int> v;
        fillVector(v, 100);
        v.clear();
    };
    auto fn2 = [](){
        std::vector<int> v;
        fillStd(v, 100);
        v.clear();
    };
    BENCH("VectorPro clear",   ITERATIONS, fn1());
    BENCH("std::vector clear", ITERATIONS, fn2());
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
    BENCH("VectorPro reserve",   ITERATIONS, fn1());
    BENCH("std::vector reserve", ITERATIONS, fn2());
}

static void shrink_to_fit() {
    Vector<int>      v1;
    std::vector<int> v2;

    v1.push_back(1);
    v2.push_back(1);

    BENCH("VectorPro shrink_to_fit",   ITERATIONS, (v1.reserve(100), v1.shrink_to_fit()));
    BENCH("std::vector shrink_to_fit", ITERATIONS, (v2.reserve(100), v2.shrink_to_fit()));
}

void run_modifier_benchmarks() {
    bench_header("Modifier");

    push_back_copy();
    std::cout << "\n";

    push_back_move();
    std::cout << "\n";

    emplace_back();
    std::cout << "\n";

    insert_single();
    std::cout << "\n";

    insert_range();
    std::cout << "\n";

    emplace();
    std::cout << "\n";

    remove_if();
    std::cout << "\n";

    pop_back();
    std::cout << "\n";

    erase_single();
    std::cout << "\n";

    erase_range();
    std::cout << "\n";

    clear();
    std::cout << "\n";

    reserve();
    std::cout << "\n";

    shrink_to_fit();
    bench_footer();
}