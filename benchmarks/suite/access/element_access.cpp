// Vector Element Access Benchmark Suite
// Measures single-element accessor performance against std::vector.
//
// Covers:
// - at() bounds-checked access
// - front() / back()
// - raw pointer access via data_ptr() / data()

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures at() bounds-checked access over a full pass.
static void bench_at() {
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
            sum += v.at(i);
        doNotOptimize(sum);
    };
    BENCH("VectorPro at()", SMALL, vp);

    auto svf = [&] {
        long sum = 0;
        for (std::size_t i = 0; i < sv.size(); ++i)
            sum += sv.at(i);
        doNotOptimize(sum);
    };
    BENCH("std::vector at()", SMALL, svf);
}

// Measures repeated front()/back() access.
static void bench_front_back() {
    Vector<int> v(1000, 7);
    std::vector<int> sv(1000, 7);

    auto vp = [&] {
        long sum = 0;
        for (int i = 0; i < 1000; ++i)
            sum += v.front() + v.back();
        doNotOptimize(sum);
    };
    BENCH("VectorPro front/back", SMALL, vp);

    auto svf = [&] {
        long sum = 0;
        for (int i = 0; i < 1000; ++i)
            sum += sv.front() + sv.back();
        doNotOptimize(sum);
    };
    BENCH("std::vector front/back", SMALL, svf);
}

// Measures raw pointer traversal via data_ptr() / data().
static void bench_data_ptr() {
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
        const int* p = v.data_ptr();
        for (std::size_t i = 0; i < v.size(); ++i)
            sum += p[i];
        doNotOptimize(sum);
    };
    BENCH("VectorPro data_ptr()", SMALL, vp);

    auto svf = [&] {
        long sum = 0;
        const int* p = sv.data();
        for (std::size_t i = 0; i < sv.size(); ++i)
            sum += p[i];
        doNotOptimize(sum);
    };
    BENCH("std::vector data()", SMALL, svf);
}

// Executes all element access benchmark cases.
static void run_benchmarks() {
    bench_at();
    std::cout << "\n";

    bench_front_back();
    std::cout << "\n";

    bench_data_ptr();
}

REGISTER_BENCH_SUITE();
