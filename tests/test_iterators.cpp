// Vector Iterator Test Suite
// Validates forward, reverse, and const iterator behavior
// across traversal, arithmetic, and comparison operations.
//
// Covers:
// - begin() and end()
// - cbegin() and cend()
// - rbegin() and rend()
// - crbegin() and crend()
// - iterator increment and decrement
// - iterator arithmetic
// - iterator comparison
// - range-based for loop
// - std::distance
// - std::sort via iterators

#include "test_helper.h"
#include "Vector.h"

#include <algorithm>
#include <numeric>

using namespace VectorPro;

// Begin End Traversal
// Verifies forward traversal via begin() and end().
static void begin_end_traversal() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
        sum += *it;
    CHK(sum == 15);
}

// Cbegin Cend Traversal
// Verifies const forward traversal via cbegin() and cend().
static void cbegin_cend_traversal() {
    const Vector<int> v = { 1, 2, 3 };
    int sum = 0;
    for (auto it = v.cbegin(); it != v.cend(); ++it)
        sum += *it;
    CHK(sum == 6);
}

// Rbegin Rend Traversal
// Verifies reverse traversal via rbegin() and rend().
static void rbegin_rend_traversal() {
    Vector<int> v = { 1, 2, 3 };
    Vector<int> reversed;
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        reversed.push_back(*it);
    CHK(reversed[0] == 3);
    CHK(reversed[1] == 2);
    CHK(reversed[2] == 1);
}

// Crbegin Crend Traversal
// Verifies const reverse traversal via crbegin() and crend().
static void crbegin_crend_traversal() {
    const Vector<int> v = { 1, 2, 3 };
    int sum = 0;
    for (auto it = v.crbegin(); it != v.crend(); ++it)
        sum += *it;
    CHK(sum == 6);
}

// Iterator Pre Increment
// Verifies pre-increment advances the iterator correctly.
static void iterator_pre_increment() {
    Vector<int> v = { 10, 20, 30 };
    auto it = v.begin();
    ++it;
    CHK(*it == 20);
}

// Iterator Post Increment
// Verifies post-increment returns the old position.
static void iterator_post_increment() {
    Vector<int> v = { 10, 20, 30 };
    auto it  = v.begin();
    auto old = it++;
    CHK(*old == 10);
    CHK(*it  == 20);
}

// Iterator Pre Decrement
// Verifies pre-decrement moves the iterator back correctly.
static void iterator_pre_decrement() {
    Vector<int> v = { 10, 20, 30 };
    auto it = v.end();
    --it;
    CHK(*it == 30);
}

// Iterator Post Decrement
// Verifies post-decrement returns the old position.
static void iterator_post_decrement() {
    Vector<int> v = { 10, 20, 30 };
    auto it  = v.end();
    --it;
    auto old = it--;
    CHK(*old == 30);
    CHK(*it  == 20);
}

// Iterator Arithmetic Add
// Verifies iterator advances correctly with operator+.
static void iterator_arithmetic_add() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    auto it = v.begin() + 3;
    CHK(*it == 4);
}

// Iterator Arithmetic Subtract
// Verifies iterator moves back correctly with operator-.
static void iterator_arithmetic_subtract() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    auto it = v.end() - 2;
    CHK(*it == 4);
}

// Iterator Distance
// Verifies distance between two iterators is correct.
static void iterator_distance() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    auto dist = v.end() - v.begin();
    CHK(dist == 5);
}

// Iterator Comparison Equal
// Verifies two iterators pointing to the same element are equal.
static void iterator_comparison_equal() {
    Vector<int> v = { 1, 2, 3 };
    auto a = v.begin();
    auto b = v.begin();
    CHK(a == b);
}

// Iterator Comparison Not Equal
// Verifies two iterators pointing to different elements are not equal.
static void iterator_comparison_not_equal() {
    Vector<int> v = { 1, 2, 3 };
    auto a = v.begin();
    auto b = v.end();
    CHK(a != b);
}

// Iterator Comparison Less Than
// Verifies begin() is less than end().
static void iterator_comparison_less_than() {
    Vector<int> v = { 1, 2, 3 };
    CHK(v.begin() < v.end());
}

// Range Based For
// Verifies range-based for loop works correctly.
static void range_based_for() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    int sum = 0;
    for (const auto& x : v)
        sum += x;
    CHK(sum == 15);
}

// Std Distance
// Verifies std::distance works correctly with the iterator.
static void std_distance() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    CHK(std::distance(v.begin(), v.end()) == 5);
}

// Std Sort
// Verifies std::sort works correctly via iterators.
static void std_sort() {
    Vector<int> v = { 5, 3, 1, 4, 2 };
    std::sort(v.begin(), v.end());
    CHK(v[0] == 1);
    CHK(v[2] == 3);
    CHK(v[4] == 5);
}

// Std Accumulate
// Verifies std::accumulate works correctly via iterators.
static void std_accumulate() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    int sum = std::accumulate(v.begin(), v.end(), 0);
    CHK(sum == 15);
}

// Iterator Subscript
// Verifies iterator subscript operator returns correct element.
static void iterator_subscript() {
    Vector<int> v = { 10, 20, 30 };
    auto it = v.begin();
    CHK(it[0] == 10);
    CHK(it[2] == 30);
}

// Test Runner
// Executes all iterator test cases.
void run_iterator_tests() {
    setTitle("Iterator Tests");
    
    RUN(begin_end_traversal);
    RUN(cbegin_cend_traversal);
    RUN(rbegin_rend_traversal);
    RUN(crbegin_crend_traversal);
    RUN(iterator_pre_increment);
    RUN(iterator_post_increment);
    RUN(iterator_pre_decrement);
    RUN(iterator_post_decrement);
    RUN(iterator_arithmetic_add);
    RUN(iterator_arithmetic_subtract);
    RUN(iterator_distance);
    RUN(iterator_comparison_equal);
    RUN(iterator_comparison_not_equal);
    RUN(iterator_comparison_less_than);
    RUN(range_based_for);
    RUN(std_distance);
    RUN(std_sort);
    RUN(std_accumulate);
    RUN(iterator_subscript);

    std::cout << "\n";
}