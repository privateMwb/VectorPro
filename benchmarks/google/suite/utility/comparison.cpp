// Vector comparison benchmark suite.
//
// Measures comparison performance against std::vector.
//
// Coverage:
// - operator== (equal, differing early, differing late, different sizes)
// - operator<=>

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

#include <vector>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// operator== equal
// ---------------------------------------------------------------------------

static void Equal_True_VectorPro(benchmark::State& state) {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }

    for (auto _ : state) {
        bool r = (a == b);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Equal_True_VectorPro);

static void Equal_True_StdVector(benchmark::State& state) {
    std::vector<int> sa, sb;
    sa.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        sa.push_back(i);
    }
    sb = sa;

    for (auto _ : state) {
        bool r = (sa == sb);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Equal_True_StdVector);

// ---------------------------------------------------------------------------
// operator== differs early
// ---------------------------------------------------------------------------

static void Equal_DiffersEarly_VectorPro(benchmark::State& state) {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }
    b[0] = -1;

    for (auto _ : state) {
        bool r = (a == b);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Equal_DiffersEarly_VectorPro);

static void Equal_DiffersEarly_StdVector(benchmark::State& state) {
    std::vector<int> sa, sb;
    sa.reserve(10'000);
    sb.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        sa.push_back(i);
        sb.push_back(i);
    }
    sb[0] = -1;

    for (auto _ : state) {
        bool r = (sa == sb);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Equal_DiffersEarly_StdVector);

// ---------------------------------------------------------------------------
// operator== differs late
// ---------------------------------------------------------------------------

static void Equal_DiffersLate_VectorPro(benchmark::State& state) {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }
    b[9999] = -1;

    for (auto _ : state) {
        bool r = (a == b);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Equal_DiffersLate_VectorPro);

static void Equal_DiffersLate_StdVector(benchmark::State& state) {
    std::vector<int> sa, sb;
    sa.reserve(10'000);
    sb.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        sa.push_back(i);
        sb.push_back(i);
    }
    sb[9999] = -1;

    for (auto _ : state) {
        bool r = (sa == sb);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Equal_DiffersLate_StdVector);

// ---------------------------------------------------------------------------
// operator== different sizes
// ---------------------------------------------------------------------------

static void Equal_DifferentSizes_VectorPro(benchmark::State& state) {
    Vector<int> a(10'000, 0);
    Vector<int> b(5'000, 0);

    for (auto _ : state) {
        bool r = (a == b);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Equal_DifferentSizes_VectorPro);

static void Equal_DifferentSizes_StdVector(benchmark::State& state) {
    std::vector<int> sa(10'000, 0);
    std::vector<int> sb(5'000, 0);

    for (auto _ : state) {
        bool r = (sa == sb);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Equal_DifferentSizes_StdVector);

// ---------------------------------------------------------------------------
// operator<=> equal
// ---------------------------------------------------------------------------

static void Spaceship_Equal_VectorPro(benchmark::State& state) {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }

    for (auto _ : state) {
        auto r = (a <=> b);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Spaceship_Equal_VectorPro);

static void Spaceship_Equal_StdVector(benchmark::State& state) {
    std::vector<int> sa, sb;
    sa.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        sa.push_back(i);
    }
    sb = sa;

    for (auto _ : state) {
        auto r = (sa <=> sb);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Spaceship_Equal_StdVector);

// ---------------------------------------------------------------------------
// operator<=> less
// ---------------------------------------------------------------------------

static void Spaceship_Less_VectorPro(benchmark::State& state) {
    Vector<int> a, b;
    a.reserve(10'000);
    b.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        a.push_back(i);
        b.push_back(i);
    }
    a[0] = -1;

    for (auto _ : state) {
        auto r = (a <=> b);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Spaceship_Less_VectorPro);

static void Spaceship_Less_StdVector(benchmark::State& state) {
    std::vector<int> sa, sb;
    sa.reserve(10'000);
    sb.reserve(10'000);
    for (int i = 0; i < 10'000; ++i) {
        sa.push_back(i);
        sb.push_back(i);
    }
    sa[0] = -1;

    for (auto _ : state) {
        auto r = (sa <=> sb);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(Spaceship_Less_StdVector);
