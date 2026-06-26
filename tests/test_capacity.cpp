// Vector Capacity Test Suite
// Validates size, capacity, and empty state reporting
// across various vector states and growth scenarios.
//
// Covers:
// - empty() on default constructed vector
// - empty() on non-empty vector
// - size() after insertions and removals
// - capacity() after reserve
// - capacity() growth on push_back
// - capacity() with custom growth factor
// - capacity() unchanged after clear
// - capacity() reduced after shrink_to_fit

#include "test_helper.h"
#include "Vector.h"

using namespace VectorPro;

// Empty Default
// Verifies a default constructed vector reports empty.
static void empty_default() {
    Vector<int> v;
    CHK(v.empty());
}

// Empty Non Empty
// Verifies a vector with elements does not report empty.
static void empty_non_empty() {
    Vector<int> v = { 1, 2, 3 };
    CHK(!v.empty());
}

// Empty After Clear
// Verifies a vector reports empty after clear.
static void empty_after_clear() {
    Vector<int> v = { 1, 2, 3 };
    v.clear();
    CHK(v.empty());
}

// Size After Push Back
// Verifies size increments correctly after each push_back.
static void size_after_push_back() {
    Vector<int> v;
    CHK(v.size() == 0);
    v.push_back(1);
    CHK(v.size() == 1);
    v.push_back(2);
    CHK(v.size() == 2);
}

// Size After Pop Back
// Verifies size decrements correctly after pop_back.
static void size_after_pop_back() {
    Vector<int> v = { 1, 2, 3 };
    v.pop_back();
    CHK(v.size() == 2);
}

// Size After Erase
// Verifies size decrements correctly after erase.
static void size_after_erase() {
    Vector<int> v = { 1, 2, 3 };
    (void)v.erase(v.cbegin());
    CHK(v.size() == 2);
}

// Size After Clear
// Verifies size is zero after clear.
static void size_after_clear() {
    Vector<int> v = { 1, 2, 3 };
    v.clear();
    CHK(v.size() == 0);
}

// Capacity After Reserve
// Verifies capacity is at least the reserved amount.
static void capacity_after_reserve() {
    Vector<int> v;
    v.reserve(20);
    CHK(v.capacity() >= 20);
}

// Capacity Unchanged Below Current
// Verifies reserve does not reduce capacity.
static void capacity_unchanged_below_current() {
    Vector<int> v;
    v.reserve(20);
    std::size_t cap = v.capacity();
    v.reserve(5);
    CHK(v.capacity() == cap);
}

// Capacity Growth Default
// Verifies capacity doubles with default growth factor.
static void capacity_growth_default() {
    Vector<int> v;
    v.reserve(8);
    std::size_t cap = v.capacity();

    for (int i = 0; i < 9; ++i)
        v.push_back(i);

    CHK(v.capacity() >= cap * 2);
}

// Capacity Growth Custom
// Verifies capacity grows at 1.5x with custom growth factor.
static void capacity_growth_custom() {
    Vector<int, std::allocator<int>, 3, 2> v;

    // fill exactly to INITIAL_CAP to force one reallocation
    for (int i = 0; i < 9; ++i)
        v.push_back(i);

    // INITIAL_CAP is 8, after reallocation: 8 * 3 / 2 = 12
    CHK(v.capacity() == 12);
    CHK(v.size()     == 9);
}

// Capacity Unchanged After Clear
// Verifies clear does not reduce capacity.
static void capacity_unchanged_after_clear() {
    Vector<int> v = { 1, 2, 3 };
    v.reserve(20);
    std::size_t cap = v.capacity();
    v.clear();
    CHK(v.capacity() == cap);
}

// Capacity After Shrink To Fit
// Verifies shrink_to_fit reduces capacity to match size.
static void capacity_after_shrink_to_fit() {
    Vector<int> v = { 1, 2, 3 };
    v.reserve(20);
    v.shrink_to_fit();
    CHK(v.capacity() == v.size());
}

// Test Runner
// Executes all capacity test cases.
void run_capacity_tests() {
    setTitle("Capacity Tests");

    RUN(empty_default);
    RUN(empty_non_empty);
    RUN(empty_after_clear);
    RUN(size_after_push_back);
    RUN(size_after_pop_back);
    RUN(size_after_erase);
    RUN(size_after_clear);
    RUN(capacity_after_reserve);
    RUN(capacity_unchanged_below_current);
    RUN(capacity_growth_default);
    RUN(capacity_growth_custom);
    RUN(capacity_unchanged_after_clear);
    RUN(capacity_after_shrink_to_fit);

    std::cout << "\n";
}