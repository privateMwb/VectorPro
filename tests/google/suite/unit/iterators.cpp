// Vector Iterator Test Suite
// Verifies iterator traversal, conversions, and iterator requirements.
//
// Covers:
// - begin/end
// - const begin/end
// - cbegin/cend
// - rbegin/rend
// - crbegin/crend
// - range-based iteration
// - iterator mutation
// - iterator to const_iterator conversion
// - random-access iterator operations
// - contiguous_iterator conformance

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies begin() and end() delimit the full element range.
TEST(Iterator, BeginEndBasic) {
    Vector<int> v{1, 2, 3};
    auto it = v.begin();

    EXPECT_EQ(*it, 1);
    EXPECT_EQ(v.end() - v.begin(), 3);
}

// Verifies begin() equals end() for an empty vector.
TEST(Iterator, BeginEqualsEndWhenEmpty) {
    Vector<int> v;
    EXPECT_EQ(v.begin(), v.end());
}

// Verifies range-based for iteration visits every element.
TEST(Iterator, RangeForIteratesAllElements) {
    Vector<int> v{1, 2, 3};
    int sum = 0;
    for (int x : v)
        sum += x;

    EXPECT_EQ(sum, 6);
}

// Verifies mutable iterators provide writable element access.
TEST(Iterator, IteratorMutatesElements) {
    Vector<int> v{1, 2, 3};
    for (auto it = v.begin(); it != v.end(); ++it)
        *it *= 10;

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

// Verifies const begin() and end() provide read-only iteration.
TEST(Iterator, ConstBeginEnd) {
    const Vector<int> v{1, 2, 3};
    EXPECT_EQ(*v.begin(), 1);
    EXPECT_EQ(v.end() - v.begin(), 3);
}

// Verifies cbegin() and cend() provide const iterators.
TEST(Iterator, CbeginCendBasic) {
    Vector<int> v{1, 2, 3};
    auto it = v.cbegin();

    EXPECT_EQ(*it, 1);
    EXPECT_EQ(v.cend() - v.cbegin(), 3);
}

// Verifies reverse iteration starts from the last element.
TEST(Iterator, RbeginRendOrder) {
    Vector<int> v{1, 2, 3};
    auto it = v.rbegin();

    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 2);
}

// Verifies reverse iteration visits every element in reverse order.
TEST(Iterator, RbeginRendFullTraversal) {
    Vector<int> v{1, 2, 3};
    int expected = 3;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        EXPECT_EQ(*it, expected);
        --expected;
    }
}

// Verifies crbegin() and crend() provide const reverse iterators.
TEST(Iterator, CrbeginCrendBasic) {
    const Vector<int> v{1, 2, 3};
    auto it = v.crbegin();

    EXPECT_EQ(*it, 3);
    EXPECT_EQ(v.crend() - v.crbegin(), 3);
}

// Verifies iterator converts implicitly to const_iterator.
TEST(Iterator, IteratorToConstIteratorConversion) {
    Vector<int> v{1, 2, 3};
    Vector<int>::iterator it = v.begin();
    Vector<int>::const_iterator cit = it;

    EXPECT_EQ(*cit, 1);
}

// Verifies iterator arithmetic supports random-access traversal.
TEST(Iterator, IteratorArithmetic) {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.begin() + 2;

    EXPECT_EQ(*it, 3);
    it -= 1;
    EXPECT_EQ(*it, 2);
}

// Verifies iterators support indexed random-access operations.
TEST(Iterator, IteratorRandomAccessOffset) {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.begin();

    EXPECT_EQ(it[3], 4);
}

// Verifies iterator comparison operators behave correctly.
TEST(Iterator, IteratorComparison) {
    Vector<int> v{1, 2, 3};
    auto a = v.begin();
    auto b = v.begin() + 1;

    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
    EXPECT_NE(a, b);
}

// Verifies the iterator types satisfy the contiguous_iterator concept.
TEST(Iterator, SatisfiesContiguousIteratorConcept) {
    static_assert(std::contiguous_iterator<Vector<int>::iterator>);
    static_assert(std::contiguous_iterator<Vector<int>::const_iterator>);
    SUCCEED();
}
