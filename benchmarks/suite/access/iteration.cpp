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

#include <support/framework.h>

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

    auto svf = [&] {
        long sum = 0;
        for (int x : sv)
            sum += x;
        doNotOptimize(sum);
    };

    BENCH("range-for", vp, svf);
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

    auto svf = [&] {
        long sum = 0;
        for (auto it = sv.begin(); it != sv.end(); ++it)
            sum += *it;
        doNotOptimize(sum);
    };

    BENCH("iterator loop", vp, svf);
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

    auto svf = [&] {
        long sum = 0;
        for (std::size_t i = 0; i < sv.size(); ++i)
            sum += sv[i];
        doNotOptimize(sum);
    };

    BENCH("index loop", vp, svf);
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

    auto svf = [&] {
        long sum = 0;
        for (auto it = sv.rbegin(); it != sv.rend(); ++it)
            sum += *it;
        doNotOptimize(sum);
    };

    BENCH("reverse iteration", vp, svf);
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

    auto svf = [&] {
        long sum = std::accumulate(sv.begin(), sv.end(), 0L);
        doNotOptimize(sum);
    };

    BENCH("accumulate", vp, svf);
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
