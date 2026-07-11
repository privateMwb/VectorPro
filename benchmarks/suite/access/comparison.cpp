// Vector comparison benchmark suite.
//
// Measures comparison performance against std::vector.
//
// Coverage:
// - operator== (equal, differing early, differing late, different sizes)
// - operator<=>

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures operator== on equal vectors.
static void bench_equal_true() {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }

    std::vector<int> sa(a.as_span().begin(), a.as_span().end());
    std::vector<int> sb(sa);

    auto vp = [&] {
        bool r = (a == b);
        doNotOptimize(r);
    };
    BENCH("VectorPro operator== equal", SMALL, vp);

    auto sv = [&] {
        bool r = (sa == sb);
        doNotOptimize(r);
    };
    BENCH("std::vector operator== equal", SMALL, sv);
}

// Measures operator== with an early mismatch.
static void bench_equal_differs_early() {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }

    b[0] = -1;

    std::vector<int> sa(a.as_span().begin(), a.as_span().end());
    std::vector<int> sb(b.as_span().begin(), b.as_span().end());

    auto vp = [&] {
        bool r = (a == b);
        doNotOptimize(r);
    };
    BENCH("VectorPro operator== differs early", SMALL, vp);

    auto sv = [&] {
        bool r = (sa == sb);
        doNotOptimize(r);
    };
    BENCH("std::vector operator== differs early", SMALL, sv);
}

// Measures operator== with a late mismatch.
static void bench_equal_differs_late() {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }

    b[9999] = -1;

    std::vector<int> sa(a.as_span().begin(), a.as_span().end());
    std::vector<int> sb(b.as_span().begin(), b.as_span().end());

    auto vp = [&] {
        bool r = (a == b);
        doNotOptimize(r);
    };
    BENCH("VectorPro operator== differs late", SMALL, vp);

    auto sv = [&] {
        bool r = (sa == sb);
        doNotOptimize(r);
    };
    BENCH("std::vector operator== differs late", SMALL, sv);
}

// Measures operator== on vectors with different sizes.
static void bench_equal_different_sizes() {
    Vector<int> a(10'000, 0);
    Vector<int> b(5'000, 0);

    std::vector<int> sa(10'000, 0);
    std::vector<int> sb(5'000, 0);

    auto vp = [&] {
        bool r = (a == b);
        doNotOptimize(r);
    };
    BENCH("VectorPro operator== diff sizes", LARGE, vp);

    auto sv = [&] {
        bool r = (sa == sb);
        doNotOptimize(r);
    };
    BENCH("std::vector operator== diff sizes", LARGE, sv);
}

// Measures operator<=> on equal vectors.
static void bench_spaceship_equal() {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }

    std::vector<int> sa(a.as_span().begin(), a.as_span().end());
    std::vector<int> sb(sa);

    auto vp = [&] {
        auto r = (a <=> b);
        doNotOptimize(r);
    };
    BENCH("VectorPro operator<=> equal", SMALL, vp);

    auto sv = [&] {
        auto r = (sa <=> sb);
        doNotOptimize(r);
    };
    BENCH("std::vector operator<=> equal", SMALL, sv);
}

// Measures operator<=> where the left vector compares less.
static void bench_spaceship_less() {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }

    a[0] = -1;

    std::vector<int> sa(a.as_span().begin(), a.as_span().end());
    std::vector<int> sb(b.as_span().begin(), b.as_span().end());

    auto vp = [&] {
        auto r = (a <=> b);
        doNotOptimize(r);
    };
    BENCH("VectorPro operator<=> less", SMALL, vp);

    auto sv = [&] {
        auto r = (sa <=> sb);
        doNotOptimize(r);
    };
    BENCH("std::vector operator<=> less", SMALL, sv);
}

// Executes all comparison benchmark cases.
static void run_benchmarks() {
    bench_equal_true();
    std::cout << "\n";

    bench_equal_differs_early();
    std::cout << "\n";

    bench_equal_differs_late();
    std::cout << "\n";

    bench_equal_different_sizes();
    std::cout << "\n";

    bench_spaceship_equal();
    std::cout << "\n";

    bench_spaceship_less();
}

REGISTER_BENCH_SUITE();