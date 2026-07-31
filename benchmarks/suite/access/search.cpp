// Vector Search Benchmark Suite
// Measures search performance against equivalent std::vector operations.
//
// Covers:
// - contains() hit
// - contains() miss
// - contains() hit on a large vector
// - find() hit
// - find() miss

#include <support/framework.h>

#include <algorithm>
#include <vector>

using namespace VectorPro;

// Measures contains() on a hit near the middle of a small vector.
static void bench_contains_hit_small() {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        bool r = v.contains(500);
        doNotOptimize(r);
    };

    auto svf = [&] {
        bool r = std::find(sv.begin(), sv.end(), 500) != sv.end();
        doNotOptimize(r);
    };

    BENCH("contains hit (1k)", vp, svf);
}

// Measures contains() on a miss, requiring a full linear scan.
static void bench_contains_miss_small() {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        bool r = v.contains(-1);
        doNotOptimize(r);
    };

    auto svf = [&] {
        bool r = std::find(sv.begin(), sv.end(), -1) != sv.end();
        doNotOptimize(r);
    };

    BENCH("contains miss (1k)", vp, svf);
}

// Measures contains() on a hit near the middle of a large vector.
static void bench_contains_hit_large() {
    Vector<int> v;
    v.reserve(10'000);
    for (int i = 0; i < 10'000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(10'000);
    for (int i = 0; i < 10'000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        bool r = v.contains(10'000);
        doNotOptimize(r);
    };

    auto svf = [&] {
        bool r = std::find(sv.begin(), sv.end(), 10'000) != sv.end();
        doNotOptimize(r);
    };

    BENCH("contains hit (10k)", vp, svf);
}

// Measures find() on a hit near the middle of a small vector.
static void bench_find_hit_small() {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        auto it = v.find(500);
        doNotOptimize(it);
    };

    auto svf = [&] {
        auto it = std::find(sv.begin(), sv.end(), 500);
        doNotOptimize(it);
    };

    BENCH("find hit (1k)", vp, svf);
}

// Measures find() on a miss, requiring a full linear scan.
static void bench_find_miss_small() {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        auto it = v.find(-1);
        doNotOptimize(it);
    };

    auto svf = [&] {
        auto it = std::find(sv.begin(), sv.end(), -1);
        doNotOptimize(it);
    };
    BENCH("find miss (1k)", vp, svf);
}

// Executes all search benchmark cases.
static void run_benchmarks() {
    bench_contains_hit_small();
    std::cout << "\n";

    bench_contains_miss_small();
    std::cout << "\n";

    bench_contains_hit_large();
    std::cout << "\n";

    bench_find_hit_small();
    std::cout << "\n";

    bench_find_miss_small();
}

REGISTER_BENCH_SUITE();
