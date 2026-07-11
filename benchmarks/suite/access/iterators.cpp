// Vector iteration benchmark suite.
//
// Measures iteration performance against std::vector.
//
// Coverage:
// - Range-for iteration
// - Iterator traversal
// - Index-based traversal
// - Reverse iteration
// - std::accumulate

#include <common/framework.h>

#include <numeric>
#include <vector>

using namespace VectorPro;

// Measures range-for iteration over 100k elements.
static void bench_range_for() {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        long sum = 0;
        for (int x : v)
            sum += x;
        doNotOptimize(sum);
    };
    BENCH("VectorPro range-for", SMALL, vp);

    auto svf = [&] {
        long sum = 0;
        for (int x : sv)
            sum += x;
        doNotOptimize(sum);
    };
    BENCH("std::vector range-for", SMALL, svf);
}

// Measures traversal using forward iterators.
static void bench_iterator_loop() {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        long sum = 0;
        for (auto it = v.begin(); it != v.end(); ++it)
            sum += *it;
        doNotOptimize(sum);
    };
    BENCH("VectorPro iterator loop", SMALL, vp);

    auto svf = [&] {
        long sum = 0;
        for (auto it = sv.begin(); it != sv.end(); ++it)
            sum += *it;
        doNotOptimize(sum);
    };
    BENCH("std::vector iterator loop", SMALL, svf);
}

// Measures index-based traversal using operator[].
static void bench_index_loop() {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        long sum = 0;
        for (std::size_t i = 0; i < v.size(); ++i)
            sum += v[i];
        doNotOptimize(sum);
    };
    BENCH("VectorPro index loop", SMALL, vp);

    auto svf = [&] {
        long sum = 0;
        for (std::size_t i = 0; i < sv.size(); ++i)
            sum += sv[i];
        doNotOptimize(sum);
    };
    BENCH("std::vector index loop", SMALL, svf);
}

// Measures traversal using reverse iterators.
static void bench_reverse_iteration() {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        long sum = 0;
        for (auto it = v.rbegin(); it != v.rend(); ++it)
            sum += *it;
        doNotOptimize(sum);
    };
    BENCH("VectorPro reverse iteration", SMALL, vp);

    auto svf = [&] {
        long sum = 0;
        for (auto it = sv.rbegin(); it != sv.rend(); ++it)
            sum += *it;
        doNotOptimize(sum);
    };
    BENCH("std::vector reverse iteration", SMALL, svf);
}

// Measures compatibility with std::accumulate.
static void bench_accumulate() {
    Vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        v.push_back(i);

    std::vector<int> sv;
    sv.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        sv.push_back(i);

    auto vp = [&] {
        long sum = std::accumulate(v.begin(), v.end(), 0L);
        doNotOptimize(sum);
    };
    BENCH("VectorPro accumulate", SMALL, vp);

    auto svf = [&] {
        long sum = std::accumulate(sv.begin(), sv.end(), 0L);
        doNotOptimize(sum);
    };
    BENCH("std::vector accumulate", SMALL, svf);
}

// Executes all iteration benchmark cases.
static void run_benchmarks() {
    bench_range_for();
    std::cout << "\n";

    bench_iterator_loop();
    std::cout << "\n";

    bench_index_loop();
    std::cout << "\n";

    bench_reverse_iteration();
    std::cout << "\n";

    bench_accumulate();
}

REGISTER_BENCH_SUITE();