// Vector Capacity Test Suite
// Verifies size, capacity, and growth behavior.
//
// Covers:
// - empty vector state
// - size tracking
// - capacity reporting
// - capacity growth
// - capacity preservation after element removal

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies empty() returns true for a default-constructed vector.
TEST(Capacity, EmptyOnDefaultConstruction) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
}

// Verifies empty() returns false after inserting an element.
TEST(Capacity, NotEmptyAfterPushBack) {
    Vector<int> v;
    v.push_back(1);
    EXPECT_FALSE(v.empty());
}

// Verifies empty() returns true after clearing the vector.
TEST(Capacity, EmptyAfterClear) {
    Vector<int> v{1, 2, 3};
    v.clear();
    EXPECT_TRUE(v.empty());
}

// Verifies size() tracks the number of inserted elements.
TEST(Capacity, SizeTracksElements) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    EXPECT_EQ(v.size(), 3u);
}

// Verifies size() decreases after removing the last element.
TEST(Capacity, SizeDecreasesOnPopBack) {
    Vector<int> v{1, 2, 3};
    v.pop_back();
    EXPECT_EQ(v.size(), 2u);
}

// Verifies a default-constructed vector has zero capacity.
TEST(Capacity, CapacityZeroOnDefaultConstruction) {
    Vector<int> v;
    EXPECT_EQ(v.capacity(), 0u);
}

// Verifies capacity() is never smaller than size().
TEST(Capacity, CapacityAtLeastSize) {
    Vector<int> v{1, 2, 3};
    EXPECT_GE(v.capacity(), v.size());
}

// Verifies capacity() grows as elements are appended.
TEST(Capacity, CapacityGrowsOnOverflow) {
    Vector<int> v;
    std::size_t lastCap = v.capacity();
    bool grew = false;

    for (int i = 0; i < 50; ++i) {
        v.push_back(i);
        if (v.capacity() > lastCap) {
            grew = true;
            lastCap = v.capacity();
        }
    }

    EXPECT_TRUE(grew);
}

// Verifies pop_back() does not reduce capacity().
TEST(Capacity, CapacityUnchangedByPopBack) {
    Vector<int> v{1, 2, 3};
    std::size_t capBefore = v.capacity();
    v.pop_back();

    EXPECT_EQ(v.capacity(), capBefore);
}

// Verifies capacity growth meets the expected expansion policy.
TEST(Capacity, CapacityGrowthRatioRespected) {
    Vector<int> v;
    v.reserve(10);
    std::size_t capBefore = v.capacity();

    for (std::size_t i = 0; i < capBefore; ++i)
        v.push_back(0);

    v.push_back(0); // Forces growth.

    EXPECT_GE(v.capacity(), capBefore * 2);
}
