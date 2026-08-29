// Vector Comparison Test Suite
// Verifies equality and three-way comparison behavior.
//
// Covers:
// - equality for identical vectors
// - inequality for different contents
// - inequality for different sizes
// - equality of empty vectors
// - lexicographical less-than comparison
// - lexicographical greater-than comparison
// - equality via three-way comparison
// - shorter prefix ordering

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies operator== returns true for vectors with identical contents.
TEST(Comparison, EqualSameContents) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 3};

    EXPECT_TRUE(a == b);
}

// Verifies operator== returns false when element values differ.
TEST(Comparison, NotEqualDifferentContents) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 4};

    EXPECT_FALSE(a == b);
}

// Verifies operator== returns false for vectors with different sizes.
TEST(Comparison, NotEqualDifferentSizes) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2};

    EXPECT_FALSE(a == b);
}

// Verifies operator== returns true for two empty vectors.
TEST(Comparison, EqualEmptyVectors) {
    Vector<int> a;
    Vector<int> b;

    EXPECT_TRUE(a == b);
}

// Verifies operator<=> performs lexicographical ordering.
TEST(Comparison, LessThanLexicographic) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 4};

    EXPECT_TRUE((a <=> b) < 0);
}

// Verifies operator<=> reports greater-than for lexicographically larger vectors.
TEST(Comparison, GreaterThanLexicographic) {
    Vector<int> a{1, 3, 3};
    Vector<int> b{1, 2, 4};

    EXPECT_TRUE((a <=> b) > 0);
}

// Verifies operator<=> reports equality for identical vectors.
TEST(Comparison, EqualSpaceship) {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 3};

    EXPECT_TRUE((a <=> b) == 0);
}

// Verifies a shorter matching prefix compares less than a longer vector.
TEST(Comparison, ShorterPrefixIsLess) {
    Vector<int> a{1, 2};
    Vector<int> b{1, 2, 3};

    EXPECT_TRUE((a <=> b) < 0);
}
