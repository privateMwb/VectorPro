// Vector Swap Test Suite
// Verifies member and non-member swap behavior.
//
// Covers:
// - member swap exchanges contents
// - member swap exchanges capacity
// - member swap with an empty vector
// - self member swap
// - free swap
// - free swap matches member swap
// - event listeners are transferred with swapped state

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies member swap exchanges the contents of both vectors.
TEST(Swap, MemberSwapExchangesContents) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{4, 5};

    a.swap(b);

    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a[0], 4);
    EXPECT_EQ(a[1], 5);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
    EXPECT_EQ(b[2], 3);
}

// Verifies member swap exchanges the capacities of both vectors.
TEST(Swap, MemberSwapExchangesCapacity) {
    Vector<int> a;
    a.reserve(100);
    Vector<int> b;
    b.reserve(10);

    std::size_t capA = a.capacity();
    std::size_t capB = b.capacity();

    a.swap(b);

    EXPECT_EQ(a.capacity(), capB);
    EXPECT_EQ(b.capacity(), capA);
}

// Verifies member swap correctly exchanges a populated vector with an empty one.
TEST(Swap, MemberSwapWithEmpty) {
    Vector<int> a{1, 2, 3};
    Vector<int> b;

    a.swap(b);

    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 1);
}

// Verifies self swap preserves the vector state.
TEST(Swap, MemberSwapSelf) {
    Vector<int> a{1, 2, 3};
    a.swap(a);

    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 3);
}

// Verifies the non-member swap exchanges vector contents.
TEST(Swap, FreeSwapExchangesContents) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{4, 5};

    swap(a, b);

    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a[0], 4);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 1);
}

// Verifies the non-member swap behaves identically to member swap.
TEST(Swap, FreeSwapMatchesMemberSwap) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{9, 8, 7, 6};

    swap(a, b);

    EXPECT_EQ(a.size(), 4u);
    EXPECT_EQ(a[0], 9);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 1);
}

// Verifies subscribed listeners remain attached to the swapped state.
TEST(Swap, SwapPreservesListeners) {
    ObservableVector<int> a{1, 2};
    ObservableVector<int> b{3, 4};

    int callCount = 0;
    (void)a.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++callCount; });

    a.swap(b);

    // Listener ownership moves with the swapped vector state.
    b.push_back(5);

    EXPECT_EQ(callCount, 1);
}
