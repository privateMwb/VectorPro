// Vector Search Test Suite
// Verifies search operations and iterator results.
//
// Covers:
// - contains
// - mutable find
// - const find
// - duplicate element lookup

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies contains() returns true when the value exists.
TEST(Search, ContainsFound) {
    Vector<int> v{1, 2, 3};
    EXPECT_TRUE(v.contains(2));
}

// Verifies contains() returns false when the value does not exist.
TEST(Search, ContainsNotFound) {
    Vector<int> v{1, 2, 3};
    EXPECT_FALSE(v.contains(99));
}

// Verifies contains() returns false for an empty vector.
TEST(Search, ContainsEmptyVector) {
    Vector<int> v;
    EXPECT_FALSE(v.contains(1));
}

// Verifies find() returns an iterator to the matching element.
TEST(Search, FindReturnsCorrectIterator) {
    Vector<int> v{10, 20, 30};
    auto it = v.find(20);

    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 20);
}

// Verifies find() returns end() when no matching element exists.
TEST(Search, FindNotFoundReturnsEnd) {
    Vector<int> v{10, 20, 30};
    auto it = v.find(99);

    EXPECT_EQ(it, v.end());
}

// Verifies the iterator returned by find() provides mutable access.
TEST(Search, FindMutatesThroughIterator) {
    Vector<int> v{10, 20, 30};
    auto it = v.find(20);
    *it = 99;

    EXPECT_EQ(v[1], 99);
}

// Verifies the const overload of find() returns an iterator to the matching element.
TEST(Search, FindConstReturnsCorrectIterator) {
    const Vector<int> v{10, 20, 30};
    auto it = v.find(30);

    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 30);
}

// Verifies the const overload of find() returns end() when no matching element exists.
TEST(Search, FindConstNotFoundReturnsEnd) {
    const Vector<int> v{10, 20, 30};
    auto it = v.find(99);

    EXPECT_EQ(it, v.end());
}

// Verifies find() returns the first matching element when duplicates exist.
TEST(Search, FindFirstMatchOnDuplicates) {
    Vector<int> v{5, 1, 5, 2};
    auto it = v.find(5);

    EXPECT_EQ(it, v.begin());
}
