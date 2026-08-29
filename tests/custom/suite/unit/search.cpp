// Vector Search Test Suite
// Verifies search operations and iterator results.
//
// Covers:
// - contains
// - mutable find
// - const find
// - duplicate element lookup

#include <support/framework.h>

using namespace VectorPro;

// Verifies contains() returns true when the value exists.
static void contains_found() {
    Vector<int> v{1, 2, 3};
    CHK(v.contains(2));
}

// Verifies contains() returns false when the value does not exist.
static void contains_not_found() {
    Vector<int> v{1, 2, 3};
    CHK(!v.contains(99));
}

// Verifies contains() returns false for an empty vector.
static void contains_empty_vector() {
    Vector<int> v;
    CHK(!v.contains(1));
}

// Verifies find() returns an iterator to the matching element.
static void find_returns_correct_iterator() {
    Vector<int> v{10, 20, 30};
    auto it = v.find(20);

    CHK(it != v.end());
    CHK(*it == 20);
}

// Verifies find() returns end() when no matching element exists.
static void find_not_found_returns_end() {
    Vector<int> v{10, 20, 30};
    auto it = v.find(99);

    CHK(it == v.end());
}

// Verifies the iterator returned by find() provides mutable access.
static void find_mutates_through_iterator() {
    Vector<int> v{10, 20, 30};
    auto it = v.find(20);
    *it = 99;

    CHK(v[1] == 99);
}

// Verifies the const overload of find() returns an iterator to the matching element.
static void find_const_returns_correct_iterator() {
    const Vector<int> v{10, 20, 30};
    auto it = v.find(30);

    CHK(it != v.end());
    CHK(*it == 30);
}

// Verifies the const overload of find() returns end() when no matching element exists.
static void find_const_not_found_returns_end() {
    const Vector<int> v{10, 20, 30};
    auto it = v.find(99);

    CHK(it == v.end());
}

// Verifies find() returns the first matching element when duplicates exist.
static void find_first_match_on_duplicates() {
    Vector<int> v{5, 1, 5, 2};
    auto it = v.find(5);

    CHK(it == v.begin());
}

// Executes all search test cases.
static void run_tests() {
    RUN(contains_found);
    RUN(contains_not_found);
    RUN(contains_empty_vector);
    RUN(find_returns_correct_iterator);
    RUN(find_not_found_returns_end);
    RUN(find_mutates_through_iterator);
    RUN(find_const_returns_correct_iterator);
    RUN(find_const_not_found_returns_end);
    RUN(find_first_match_on_duplicates);
}

REGISTER_TEST_SUITE();