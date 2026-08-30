// Vector Concurrent Read-Only Access Test Suite
// Verifies that many threads reading a single *shared* Vector instance
// concurrently -- with no thread ever mutating it -- produce correct,
// consistent results and no data races. Vector makes no thread-safety
// claims for concurrent mutation, but read-only access (no writers) should
// be safe since it touches no mutable shared state.
//
// Design note: as in concurrency/isolation.cpp, worker threads never call
// gtest's EXPECT_*/ASSERT_* macros directly (they are not documented safe
// to call from worker threads); they only read from the shared Vector and
// record their outcome into a pre-sized, per-thread-indexed slot. All
// assertions run afterward on the single test-runner thread, once every
// thread has been joined.
//
// Covers:
// - concurrent operator[] reads across many threads are all correct
// - concurrent iteration (via begin()/end()) is consistent across threads
// - concurrent find() calls are consistent across threads
// - concurrent contains() calls are consistent across threads

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

#include <numeric>
#include <thread>
#include <vector>

using namespace VectorPro;

namespace {
constexpr int kThreadCount = 16;
constexpr int kElementCount = 5000;

// Builds the shared, read-only vector used by every test in this file:
// values 0..kElementCount-1 in order.
Vector<int> make_shared_vector() {
    Vector<int> v;
    for (int i = 0; i < kElementCount; ++i)
        v.push_back(i);
    return v;
}
} // namespace

// Verifies that many threads concurrently reading every element via
// operator[] all see the correct, unaltered values.
TEST(ConcurrentReadOnly, IndexedReadsAreCorrect) {
    const Vector<int> shared = make_shared_vector();
    std::vector<long long> checksums(kThreadCount, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreadCount; ++t) {
        threads.emplace_back([&shared, &checksums, t]() {
            long long sum = 0;
            for (std::size_t i = 0; i < shared.size(); ++i)
                sum += shared[i];
            checksums[static_cast<std::size_t>(t)] = sum;
        });
    }

    for (auto& th : threads)
        th.join();

    long long expected = static_cast<long long>(kElementCount - 1) * kElementCount / 2;
    for (int t = 0; t < kThreadCount; ++t) {
        EXPECT_EQ(checksums[static_cast<std::size_t>(t)], expected);
    }
}

// Verifies that many threads concurrently iterating the shared vector via
// begin()/end() (e.g. through std::accumulate) all compute the same result.
TEST(ConcurrentReadOnly, IterationIsConsistent) {
    const Vector<int> shared = make_shared_vector();
    std::vector<long long> sums(kThreadCount, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreadCount; ++t) {
        threads.emplace_back([&shared, &sums, t]() {
            sums[static_cast<std::size_t>(t)] = std::accumulate(shared.begin(), shared.end(), 0LL);
        });
    }

    for (auto& th : threads)
        th.join();

    long long expected = static_cast<long long>(kElementCount - 1) * kElementCount / 2;
    for (int t = 0; t < kThreadCount; ++t) {
        EXPECT_EQ(sums[static_cast<std::size_t>(t)], expected);
    }
}

// Verifies that many threads concurrently calling find() on the shared
// vector all locate the same, correct position.
TEST(ConcurrentReadOnly, FindCallsAreConsistent) {
    const Vector<int> shared = make_shared_vector();
    std::vector<std::ptrdiff_t> foundOffsets(kThreadCount, -1);
    std::vector<std::thread> threads;

    const int target = kElementCount / 3;

    for (int t = 0; t < kThreadCount; ++t) {
        threads.emplace_back([&shared, &foundOffsets, t, target]() {
            auto it = shared.find(target);
            foundOffsets[static_cast<std::size_t>(t)] = it - shared.begin();
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreadCount; ++t) {
        EXPECT_EQ(foundOffsets[static_cast<std::size_t>(t)], target);
    }
}

// Verifies that many threads concurrently calling contains() on the shared
// vector all agree on membership for both present and absent values.
TEST(ConcurrentReadOnly, ContainsCallsAreConsistent) {
    const Vector<int> shared = make_shared_vector();
    std::vector<char> presentResults(kThreadCount, 0);
    std::vector<char> absentResults(kThreadCount, 1);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreadCount; ++t) {
        threads.emplace_back([&shared, &presentResults, &absentResults, t]() {
            presentResults[static_cast<std::size_t>(t)] =
                shared.contains(kElementCount / 2) ? 1 : 0;
            absentResults[static_cast<std::size_t>(t)] =
                shared.contains(kElementCount + 1000) ? 1 : 0;
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreadCount; ++t) {
        EXPECT_EQ(presentResults[static_cast<std::size_t>(t)], 1);
        EXPECT_EQ(absentResults[static_cast<std::size_t>(t)], 0);
    }
}
