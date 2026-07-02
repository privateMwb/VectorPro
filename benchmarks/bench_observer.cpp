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

#include "bench_helper.h"

using namespace VectorPro;

// Measures push_back with no subscribed listeners.
static void bench_push_back_no_listeners() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i) v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("push_back, 0 listeners", MEDIUM, vp);
}

// Measures push_back with a single subscribed listener.
static void bench_push_back_one_listener() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        (void)v.subscribe([](const Vector<int>&, Vector<int>::EventData) {});
        for (int i = 0; i < 1000; ++i) v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("push_back, 1 listener", MEDIUM, vp);
}

// Measures push_back with multiple subscribed listeners.
static void bench_push_back_many_listeners() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 8; ++i)
            (void)v.subscribe([](const Vector<int>&, Vector<int>::EventData) {});
        for (int i = 0; i < 1000; ++i) v.push_back(i);
        doNotOptimize(v);
    };
    BENCH("push_back, 8 listeners", MEDIUM, vp);
}

// Measures push_back with a listener that captures external state.
static void bench_push_back_capturing_listener() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);

        long total = 0;
        (void)v.subscribe([&total](const Vector<int>&, Vector<int>::EventData e) {
            total += e.newSize;
        });

        for (int i = 0; i < 1000; ++i) v.push_back(i);
        doNotOptimize(total);
    };
    BENCH("push_back, capturing listener", MEDIUM, vp);
}

// Measures listener registration.
static void bench_subscribe() {
    auto vp = [&] {
        Vector<int> v;
        for (int i = 0; i < 100; ++i)
            (void)v.subscribe([](const Vector<int>&, Vector<int>::EventData) {});
        doNotOptimize(v);
    };
    BENCH("subscribe x100", MEDIUM, vp);
}

// Measures listener removal.
static void bench_unsubscribe() {
    auto vp = [&] {
        Vector<int> v;

        Vector<int>::ListenerHandle handles[100];
        for (int i = 0; i < 100; ++i)
            handles[i] = v.subscribe([](const Vector<int>&, Vector<int>::EventData) {});

        for (int i = 0; i < 100; ++i)
            v.unsubscribe(handles[0]);

        doNotOptimize(v);
    };
    BENCH("unsubscribe x100", MEDIUM, vp);
}

// Executes all observer benchmark cases.
void run_observer_benchmarks() {
    setHeader("Observer Benchmarks");

    bench_push_back_no_listeners();
    std::cout << "\n";

    bench_push_back_one_listener();
    std::cout << "\n";

    bench_push_back_many_listeners();
    std::cout << "\n";

    bench_push_back_capturing_listener();
    std::cout << "\n";

    bench_subscribe();
    std::cout << "\n";

    bench_unsubscribe();
    borderLine();
    std::cout << "\n";
}