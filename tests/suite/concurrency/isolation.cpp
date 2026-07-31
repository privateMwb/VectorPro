// Vector Thread Isolation Test Suite
// Verifies that independent Vector instances, one per thread, never
// interfere with one another. Vector is not internally synchronized and
// makes no thread-safety claims for a *shared* instance, but separate
// instances must still be fully independent: no hidden shared/static
// mutable state should let one thread's operations corrupt another
// thread's vector.
//
// Design note: the test framework's CHK macro increments plain (non-atomic)
// global counters, so it is not safe to call CHK from worker threads. Every
// test here has worker threads only perform Vector operations and record
// their outcome into a pre-sized, per-thread-indexed slot; all CHK
// assertions run afterward on the single test-runner thread, once every
// thread has been joined.
//
// Covers:
// - many threads each building an independent Vector via push_back never
//   corrupts another thread's contents
// - many threads each performing a mixed insert/erase/sort sequence on
//   their own Vector produce correct, uncorrupted results
// - many threads each using their own Vector's listener system see only
//   their own notifications, never another thread's

#include <support/framework.h>

#include <algorithm>
#include <thread>
#include <vector>

using namespace VectorPro;

namespace {
constexpr int kThreadCount = 16;
constexpr int kElementsPerThread = 2000;
} // namespace

// Verifies that many threads, each building its own Vector via push_back,
// end up with exactly the contents that thread produced -- no values
// leaked in from, or lost to, another thread's vector.
static void independent_push_back_across_threads_is_uncorrupted() {
    std::vector<Vector<int>> results(kThreadCount);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreadCount; ++t) {
        threads.emplace_back([&results, t]() {
            Vector<int> local;
            for (int i = 0; i < kElementsPerThread; ++i)
                local.push_back(t * kElementsPerThread + i);
            results[static_cast<std::size_t>(t)] = std::move(local);
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreadCount; ++t) {
        const Vector<int>& v = results[static_cast<std::size_t>(t)];
        CHK(v.size() == static_cast<std::size_t>(kElementsPerThread));
        for (int i = 0; i < kElementsPerThread; ++i) {
            CHK(v[static_cast<std::size_t>(i)] == t * kElementsPerThread + i);
        }
    }
}

// Verifies that many threads, each performing a mixed insert/erase/sort
// sequence on their own Vector, all produce the correct final result with
// no cross-thread corruption.
static void independent_mixed_operations_across_threads_is_uncorrupted() {
    std::vector<Vector<int>> results(kThreadCount);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreadCount; ++t) {
        threads.emplace_back([&results, t]() {
            Vector<int> local;
            for (int i = 0; i < 500; ++i)
                local.push_back((t * 7 + i * 13) % 997);

            (void)local.insert(local.cbegin() + 100, -(100000 + t));
            (void)local.erase(local.cbegin() + 50, local.cbegin() + 60);
            std::sort(local.begin(), local.end());

            results[static_cast<std::size_t>(t)] = std::move(local);
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreadCount; ++t) {
        const Vector<int>& v = results[static_cast<std::size_t>(t)];
        CHK(v.size() == 491); // 500 + 1 insert - 10 erased
        CHK(std::is_sorted(v.begin(), v.end()));

        // The sentinel value must appear exactly once (inserted by this
        // thread, and never colliding with a legitimately-generated value
        // from any thread's stream, which always falls in [0, 996]).
        CHK(std::count(v.begin(), v.end(), -(100000 + t)) == 1);
    }
}

// Verifies that each thread's own Vector's listener system only ever sees
// notifications for that thread's own mutations, never another thread's.
static void independent_listener_counts_across_threads_is_uncorrupted() {
    std::vector<int> notificationCounts(kThreadCount, 0);
    std::vector<std::size_t> finalSizes(kThreadCount, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreadCount; ++t) {
        threads.emplace_back([&notificationCounts, &finalSizes, t]() {
            ObservableVector<int> local;
            int localCount = 0;
            (void)local.subscribe(
                [&localCount](const ObservableVector<int>&,
                              const ObservableVector<int>::EventData&) { ++localCount; });

            for (int i = 0; i < 300; ++i)
                local.push_back(i);
            local.pop_back();
            (void)local.erase(local.cbegin());

            notificationCounts[static_cast<std::size_t>(t)] = localCount;
            finalSizes[static_cast<std::size_t>(t)] = local.size();
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreadCount; ++t) {
        // 300 push_backs + 1 pop_back + 1 erase = 302 notifications, and
        // never inflated by another thread's activity.
        CHK(notificationCounts[static_cast<std::size_t>(t)] == 302);
        CHK(finalSizes[static_cast<std::size_t>(t)] == 298);
    }
}

// Executes all thread-isolation test cases.
static void run_tests() {
    RUN(independent_push_back_across_threads_is_uncorrupted);
    RUN(independent_mixed_operations_across_threads_is_uncorrupted);
    RUN(independent_listener_counts_across_threads_is_uncorrupted);
}

REGISTER_TEST_SUITE();
