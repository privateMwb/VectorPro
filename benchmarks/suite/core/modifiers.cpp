// Vector Modifiers Benchmark Suite
// Measures modifier performance against std::vector.
//
// Covers:
// - push_back
// - reserved push_back
// - emplace_back
// - insert (front, middle, end)
// - erase (front, middle, end)
// - pop_back
// - clear
// - remove_if

#include <common/framework.h>

#include <vector>

using namespace VectorPro;

// Measures push_back performance.
static void bench_push_back() {
    auto vp = [&] {
        Vector<int> v;
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("VectorPro push_back", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v;
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("std::vector push_back", SMALL, sv);
}

// Measures push_back performance with reserved capacity.
static void bench_push_back_reserved() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("VectorPro push_back reserved", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("std::vector push_back reserved", SMALL, sv);
}

// Measures emplace_back performance.
static void bench_emplace_back() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.emplace_back(i);
        doNotOptimize(v);
    };
    BENCH("VectorPro emplace_back", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.emplace_back(i);
        doNotOptimize(v);
    };
    BENCH("std::vector emplace_back", SMALL, sv);
}

// Measures insertion at the beginning.
static void bench_insert_front() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.insert(v.cbegin(), 99);
        doNotOptimize(v);
    };
    BENCH("VectorPro insert front", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.insert(v.cbegin(), 99);
        doNotOptimize(v);
    };
    BENCH("std::vector insert front", SMALL, sv);
}

// Measures insertion in the middle.
static void bench_insert_middle() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.insert(v.cbegin() + 250, 99);
        doNotOptimize(v);
    };
    BENCH("VectorPro insert middle", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.insert(v.cbegin() + 250, 99);
        doNotOptimize(v);
    };
    BENCH("std::vector insert middle", SMALL, sv);
}

// Measures insertion at the end.
static void bench_insert_end() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.insert(v.cend(), 99);
        doNotOptimize(v);
    };
    BENCH("VectorPro insert end", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.insert(v.cend(), 99);
        doNotOptimize(v);
    };
    BENCH("std::vector insert end", SMALL, sv);
}

// Measures erasure at the beginning.
static void bench_erase_front() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.erase(v.cbegin());
        doNotOptimize(v);
    };
    BENCH("VectorPro erase front", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.erase(v.cbegin());
        doNotOptimize(v);
    };
    BENCH("std::vector erase front", SMALL, sv);
}

// Measures erasure in the middle.
static void bench_erase_middle() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.erase(v.cbegin() + 250);
        doNotOptimize(v);
    };
    BENCH("VectorPro erase middle", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.erase(v.cbegin() + 250);
        doNotOptimize(v);
    };
    BENCH("std::vector erase middle", SMALL, sv);
}

// Measures erasure at the end.
static void bench_erase_end() {
    auto vp = [&] {
        Vector<int> v(500, 0);
        (void)v.erase(v.cend() - 1);
        doNotOptimize(v);
    };
    BENCH("VectorPro erase end", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(500, 0);
        v.erase(v.cend() - 1);
        doNotOptimize(v);
    };
    BENCH("std::vector erase end", SMALL, sv);
}

// Measures pop_back performance.
static void bench_pop_back() {
    auto vp = [&] {
        Vector<int> v(1000, 0);
        while (!v.empty())
            v.pop_back();
        doNotOptimize(v);
    };
    BENCH("VectorPro pop_back", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(1000, 0);
        while (!v.empty())
            v.pop_back();
        doNotOptimize(v);
    };
    BENCH("std::vector pop_back", SMALL, sv);
}

// Measures clear performance.
static void bench_clear() {
    auto vp = [&] {
        Vector<int> v(1000, 0);
        v.clear();
        doNotOptimize(v);
    };
    BENCH("VectorPro clear", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v(1000, 0);
        v.clear();
        doNotOptimize(v);
    };
    BENCH("std::vector clear", SMALL, sv);
}

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
    BENCH("VectorPro remove_if", SMALL, vp);

    auto sv = [&] {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        v.erase(std::remove_if(v.begin(), v.end(), [](const int& x) { return x % 2 == 0; }),
                v.end());
        doNotOptimize(v);
    };
    BENCH("std::vector erase-remove_if", SMALL, sv);
}

// Executes all modifier benchmark cases.
static void run_benchmarks() {
    bench_push_back();
    std::cout << "\n";

    bench_push_back_reserved();
    std::cout << "\n";

    bench_emplace_back();
    std::cout << "\n";

    bench_insert_front();
    std::cout << "\n";

    bench_insert_middle();
    std::cout << "\n";

    bench_insert_end();
    std::cout << "\n";

    bench_erase_front();
    std::cout << "\n";

    bench_erase_middle();
    std::cout << "\n";

    bench_erase_end();
    std::cout << "\n";

    bench_pop_back();
    std::cout << "\n";

    bench_clear();
    std::cout << "\n";

    bench_remove_if();
}

REGISTER_BENCH_SUITE();