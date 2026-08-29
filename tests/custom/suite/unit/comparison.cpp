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

#include <support/framework.h>

using namespace VectorPro;

// Verifies operator== returns true for vectors with identical contents.
static void equal_same_contents() {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 3};

    CHK(a == b);
}

// Verifies operator== returns false when element values differ.
static void not_equal_different_contents() {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 4};

    CHK(!(a == b));
}

// Verifies operator== returns false for vectors with different sizes.
static void not_equal_different_sizes() {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2};

    CHK(!(a == b));
}

// Verifies operator== returns true for two empty vectors.
static void equal_empty_vectors() {
    Vector<int> a;
    Vector<int> b;

    CHK(a == b);
}

// Verifies operator<=> performs lexicographical ordering.
static void less_than_lexicographic() {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 4};

    CHK((a <=> b) < 0);
}

// Verifies operator<=> reports greater-than for lexicographically larger vectors.
static void greater_than_lexicographic() {
    Vector<int> a{1, 3, 3};
    Vector<int> b{1, 2, 4};

    CHK((a <=> b) > 0);
}

// Verifies operator<=> reports equality for identical vectors.
static void equal_spaceship() {
    Vector<int> a{1, 2, 3};
    Vector<int> b{1, 2, 3};

    CHK((a <=> b) == 0);
}

// Verifies a shorter matching prefix compares less than a longer vector.
static void shorter_prefix_is_less() {
    Vector<int> a{1, 2};
    Vector<int> b{1, 2, 3};

    CHK((a <=> b) < 0);
}

// Executes all comparison test cases.
static void run_tests() {
    RUN(equal_same_contents);
    RUN(not_equal_different_contents);
    RUN(not_equal_different_sizes);
    RUN(equal_empty_vectors);
    RUN(less_than_lexicographic);
    RUN(greater_than_lexicographic);
    RUN(equal_spaceship);
    RUN(shorter_prefix_is_less);
}

REGISTER_TEST_SUITE();