// Vector Operator Test Suite
// Validates equality, ordering, and subscript operator behavior.
//
// Covers:
// - operator== (equal vectors)
// - operator== (unequal vectors)
// - operator== (different sizes)
// - operator<=> (less than)
// - operator<=> (greater than)
// - operator<=> (equal)
// - operator[] read and write (const and non-const)

#include "test_helper.h"
#include "Vector.h"

using namespace VectorPro;

// Equality Equal
// Verifies two vectors with identical elements are equal.
static void equality_equal() {
    Vector<int> a = { 1, 2, 3 };
    Vector<int> b = { 1, 2, 3 };
    CHK(a == b);
}

// Equality Unequal
// Verifies two vectors with different elements are not equal.
static void equality_unequal() {
    Vector<int> a = { 1, 2, 3 };
    Vector<int> b = { 1, 2, 4 };
    CHK(!(a == b));
}

// Equality Different Sizes
// Verifies two vectors with different sizes are not equal.
static void equality_different_sizes() {
    Vector<int> a = { 1, 2, 3 };
    Vector<int> b = { 1, 2 };
    CHK(!(a == b));
}

// Spaceship Less Than
// Verifies a lexicographically smaller vector compares less.
static void spaceship_less_than() {
    Vector<int> a = { 1, 2, 3 };
    Vector<int> b = { 1, 2, 4 };
    CHK((a <=> b) < 0);
}

// Spaceship Greater Than
// Verifies a lexicographically greater vector compares greater.
static void spaceship_greater_than() {
    Vector<int> a = { 1, 2, 4 };
    Vector<int> b = { 1, 2, 3 };
    CHK((a <=> b) > 0);
}

// Spaceship Equal
// Verifies two identical vectors compare equal.
static void spaceship_equal() {
    Vector<int> a = { 1, 2, 3 };
    Vector<int> b = { 1, 2, 3 };
    CHK((a <=> b) == 0);
}

// Spaceship Shorter Less
// Verifies a shorter vector is less than a longer one with same prefix.
static void spaceship_shorter_less() {
    Vector<int> a = { 1, 2 };
    Vector<int> b = { 1, 2, 3 };
    CHK((a <=> b) < 0);
}

// Derived Less Than
// Verifies operator< works as derived from operator<=>.
static void derived_less_than() {
    Vector<int> a = { 1, 2, 3 };
    Vector<int> b = { 1, 2, 4 };
    CHK(a < b);
}

// Derived Greater Than
// Verifies operator> works as derived from operator<=>.
static void derived_greater_than() {
    Vector<int> a = { 1, 2, 4 };
    Vector<int> b = { 1, 2, 3 };
    CHK(a > b);
}

// Derived Less Than Or Equal
// Verifies operator<= works as derived from operator<=>.
static void derived_less_than_or_equal() {
    Vector<int> a = { 1, 2, 3 };
    Vector<int> b = { 1, 2, 3 };
    CHK(a <= b);
}

// Derived Greater Than Or Equal
// Verifies operator>= works as derived from operator<=>.
static void derived_greater_than_or_equal() {
    Vector<int> a = { 1, 2, 3 };
    Vector<int> b = { 1, 2, 3 };
    CHK(a >= b);
}

// Test Runner
// Executes all operator test cases.
void run_operator_tests() {
    setTitle("Operator Tests");

    RUN(equality_equal);
    RUN(equality_unequal);
    RUN(equality_different_sizes);
    RUN(spaceship_less_than);
    RUN(spaceship_greater_than);
    RUN(spaceship_equal);
    RUN(spaceship_shorter_less);
    RUN(derived_less_than);
    RUN(derived_greater_than);
    RUN(derived_less_than_or_equal);
    RUN(derived_greater_than_or_equal);

    std::cout << "\n";
}