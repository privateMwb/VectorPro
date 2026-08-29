// Vector Thread Isolation Test Suite
// Verifies that independent Vector instances, one per thread, never
// interfere with one another. Vector is not internally synchronized and
// makes no thread-safety claims for a *shared* instance, but separate
// instances must still be fully independent: no hidden shared/static
// mutable state should let one thread's operations corrupt another
// thread's vector.
//
// Design note: gtest's EXPECT_*/ASSERT_* macros are not documented as
// safe to call from worker threads, so every test here has worker
// threads only perform Vector operations and record their outcome into
// a pre-sized, per-thread-indexed slot; all assertions run afterward on
// the single test-runner thread, once every thread has been joined.
//
// Covers:
// - many threads each building an independent Vector via push_back never
//   corrupts another thread's contents
// - many threads each performing a mixed insert/erase/sort sequence on
//   their own Vector produce correct, uncorrupted results
// - many threads each using their own Vector's listener system see only
//   their own notifications, never another thread's

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

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
TEST(ThreadIsolation, IndependentPushBackAcrossThreadsIsUncorrupted) {
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
        EXPECT_EQ(v.size(), static_cast<std::size_t>(kElementsPerThread));
        for (int i = 0; i < kElementsPerThread; ++i) {
            EXPECT_EQ(v[static_cast<std::size_t>(i)], t * kElementsPerThread + i);
        }
    }
}

// Verifies that many threads, each performing a mixed insert/erase/sort
// sequence on their own Vector, all produce the correct final result with
// no cross-thread corruption.
TEST(ThreadIsolation, IndependentMixedOperationsAcrossThreadsIsUncorrupted) {
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
        EXPECT_EQ(v.size(), 491u); // 500 + 1 insert - 10 erased
        EXPECT_TRUE(std::is_sorted(v.begin(), v.end()));

        // The sentinel value must appear exactly once (inserted by this
        // thread, and never colliding with a legitimately-generated value
        // from any thread's stream, which always falls in [0, 996]).
        EXPECT_EQ(std::count(v.begin(), v.end(), -(100000 + t)), 1);
    }
}

// Verifies that each thread's own Vector's listener system only ever sees
// notifications for that thread's own mutations, never another thread's.
TEST(ThreadIsolation, IndependentListenerCountsAcrossThreadsIsUncorrupted) {
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
        EXPECT_EQ(notificationCounts[static_cast<std::size_t>(t)], 302);
        EXPECT_EQ(finalSizes[static_cast<std::size_t>(t)], 298u);
    }
}
