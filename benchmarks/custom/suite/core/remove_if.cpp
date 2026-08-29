// Vector Remove If Benchmark Suite
// Measures remove_if() against std::vector's erase-remove idiom.
//
// Covers:
// - remove_if

#include <support/framework.h>

#include <algorithm>
#include <vector>

using namespace VectorPro;

// Measures removal of matching elements.
static void bench_remove_if() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        (void)v.remove_if([](const int& x) { return x % 2 == 0; });
        doNotOptimize(v);
    };

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        v.erase(std::remove_if(v.begin(), v.end(), [](const int& x) { return x % 2 == 0; }),
                v.end());
        doNotOptimize(v);
    };

    BENCH("erase-remove_if", vp, sv);
}

// Executes all remove_if benchmark cases.
static void run_benchmarks() {
    bench_remove_if();
}

REGISTER_BENCH_SUITE();
