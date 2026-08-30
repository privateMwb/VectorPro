// Vector observer benchmark suite.
//
// Measures observer notification overhead.
//
// Unlike the other benchmark suites, these benchmarks have no std::vector
// equivalent because the standard library provides no observer mechanism.
//
// Coverage:
// - push_back with no listeners
// - push_back with one listener
// - push_back with multiple listeners
// - push_back with a capturing listener
// - subscribe
// - unsubscribe

#include <benchmark/benchmark.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// ---------------------------------------------------------------------------
// push_back, 0 listeners
// ---------------------------------------------------------------------------

static void PushBack_NoListeners(benchmark::State& state) {
    for (auto _ : state) {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(PushBack_NoListeners);

// ---------------------------------------------------------------------------
// push_back, 1 listener
// ---------------------------------------------------------------------------

static void PushBack_OneListener(benchmark::State& state) {
    for (auto _ : state) {
        ObservableVector<int> v;
        v.reserve(1000);
        (void)v.subscribe([](const ObservableVector<int>&, ObservableVector<int>::EventData) {});
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(PushBack_OneListener);

// ---------------------------------------------------------------------------
// push_back, 8 listeners
// ---------------------------------------------------------------------------

static void PushBack_ManyListeners(benchmark::State& state) {
    for (auto _ : state) {
        ObservableVector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 8; ++i)
            (void)v.subscribe(
                [](const ObservableVector<int>&, ObservableVector<int>::EventData) {});
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(PushBack_ManyListeners);

// ---------------------------------------------------------------------------
// push_back, capturing listener
// ---------------------------------------------------------------------------

static void PushBack_CapturingListener(benchmark::State& state) {
    for (auto _ : state) {
        ObservableVector<int> v;
        v.reserve(1000);

        long total = 0;
        (void)v.subscribe([&total](const ObservableVector<int>&,
                                   ObservableVector<int>::EventData e) { total += e.newSize; });

        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        benchmark::DoNotOptimize(total);
    }
}
BENCHMARK(PushBack_CapturingListener);

// ---------------------------------------------------------------------------
// subscribe x100
// ---------------------------------------------------------------------------

static void Subscribe_x100(benchmark::State& state) {
    for (auto _ : state) {
        ObservableVector<int> v;
        for (int i = 0; i < 100; ++i)
            (void)v.subscribe(
                [](const ObservableVector<int>&, ObservableVector<int>::EventData) {});
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(Subscribe_x100);

// ---------------------------------------------------------------------------
// unsubscribe x100
// ---------------------------------------------------------------------------

static void Unsubscribe_x100(benchmark::State& state) {
    for (auto _ : state) {
        ObservableVector<int> v;

        ObservableVector<int>::ListenerHandle handles[100];
        for (int i = 0; i < 100; ++i)
            handles[i] =
                v.subscribe([](const ObservableVector<int>&, ObservableVector<int>::EventData) {});

        for (int i = 0; i < 100; ++i)
            v.unsubscribe(handles[0]);

        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(Unsubscribe_x100);
