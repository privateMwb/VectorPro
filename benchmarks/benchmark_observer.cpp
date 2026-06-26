// Vector Observer Benchmark
// Measures the real cost of the observer system under
// various listener counts, callback weights, and usage patterns.
//
// Covers:
// - push_back with no listener vs one listener
// - notification cost scaling with listener count
// - lightweight vs heavyweight callback cost
// - subscribe and unsubscribe under mutation load

#include "benchmark_helper.h"

using namespace VectorPro;

// listener overhead
static void listener_overhead() {
    Vector<int> v_none;
    Vector<int> v_one;
    (void)v_one.subscribe(countListener);

    BENCH("push_back no listener",   ITERATIONS, v_none.push_back(1));
    BENCH("push_back one listener",  ITERATIONS, v_one.push_back(1));
}

// listener count scaling
static void listener_scaling() {
    Vector<int> v1;
    Vector<int> v5;
    Vector<int> v10;

    (void)v1.subscribe(countListener);

    for (int i = 0; i < 5;  ++i) (void)v5.subscribe(countListener);
    for (int i = 0; i < 10; ++i) (void)v10.subscribe(countListener);

    BENCH("push_back 1  listener",  ITERATIONS, v1.push_back(1));
    BENCH("push_back 5  listeners", ITERATIONS, v5.push_back(1));
    BENCH("push_back 10 listeners", ITERATIONS, v10.push_back(1));
}

// lightweight vs heavyweight callback
static void callback_weight() {
    // lightweight — just increments a counter
    Vector<int> v_light;
    (void)v_light.subscribe(countListener);

    // heavyweight — does string formatting on every event
    Vector<int> v_heavy;
    (void)v_heavy.subscribe([](const Vector<int>& v, Vector<int>::EventData d) {
        std::string msg = "event=" + std::to_string(static_cast<int>(d.type))
                        + " size="  + std::to_string(d.newSize);
        (void)msg;
    });

    BENCH("push_back lightweight callback", ITERATIONS, v_light.push_back(1));
    BENCH("push_back heavyweight callback", ITERATIONS, v_heavy.push_back(1));
}

// subscribe and unsubscribe under mutation load
static void subscribe_unsubscribe_load() {
    auto fn = [](){
        Vector<int> v;
        auto h = v.subscribe(countListener);
        v.push_back(1);
        v.push_back(2);
        v.unsubscribe(h);
        v.push_back(3);
    };
    BENCH("subscribe/unsubscribe under load", ITERATIONS, fn());
}

void run_observer_benchmarks() {
    bench_header("Observer");

    listener_overhead();
    std::cout << "\n";

    listener_scaling();
    std::cout << "\n";

    callback_weight();
    std::cout << "\n";

    subscribe_unsubscribe_load();

    bench_footer();
}