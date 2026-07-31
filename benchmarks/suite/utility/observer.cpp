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

#include <support/framework.h>

using namespace VectorPro;

// Measures push_back with no subscribed listeners.
static void bench_push_back_no_listeners() {
    auto vp = [&] {
        Vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };

    BENCH_SOLO("push_back, 0 listeners", vp);
}

// Measures push_back with a single subscribed listener.
static void bench_push_back_one_listener() {
    auto vp = [&] {
        ObservableVector<int> v;
        v.reserve(1000);
        (void)v.subscribe([](const ObservableVector<int>&, ObservableVector<int>::EventData) {});
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };

    BENCH_SOLO("push_back, 1 listener", vp);
}

// Measures push_back with multiple subscribed listeners.
static void bench_push_back_many_listeners() {
    auto vp = [&] {
        ObservableVector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 8; ++i)
            (void)v.subscribe(
                [](const ObservableVector<int>&, ObservableVector<int>::EventData) {});
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(v);
    };

    BENCH_SOLO("push_back, 8 listeners", vp);
}

// Measures push_back with a listener that captures external state.
static void bench_push_back_capturing_listener() {
    auto vp = [&] {
        ObservableVector<int> v;
        v.reserve(1000);

        long total = 0;
        (void)v.subscribe([&total](const ObservableVector<int>&,
                                   ObservableVector<int>::EventData e) { total += e.newSize; });

        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        doNotOptimize(total);
    };

    BENCH_SOLO("push_back, capturing listener", vp);
}

// Measures listener registration.
static void bench_subscribe() {
    auto vp = [&] {
        ObservableVector<int> v;
        for (int i = 0; i < 100; ++i)
            (void)v.subscribe(
                [](const ObservableVector<int>&, ObservableVector<int>::EventData) {});
        doNotOptimize(v);
    };

    BENCH_SOLO("subscribe x100", vp);
}

// Measures listener removal.
static void bench_unsubscribe() {
    auto vp = [&] {
        ObservableVector<int> v;

        ObservableVector<int>::ListenerHandle handles[100];
        for (int i = 0; i < 100; ++i)
            handles[i] =
                v.subscribe([](const ObservableVector<int>&, ObservableVector<int>::EventData) {});

        for (int i = 0; i < 100; ++i)
            v.unsubscribe(handles[0]);

        doNotOptimize(v);
    };
    BENCH_SOLO("unsubscribe x100", vp);
}

// Executes all observer benchmark cases.
static void run_benchmarks() {
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
}

REGISTER_BENCH_SUITE();