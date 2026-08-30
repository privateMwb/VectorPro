// Vector Capacity Test Suite
// Verifies size, capacity, and growth behavior.
//
// Covers:
// - empty vector state
// - size tracking
// - capacity reporting
// - capacity growth
// - capacity preservation after element removal

#include <support/framework.h>

using namespace VectorPro;

// Verifies empty() returns true for a default-constructed vector.
static void empty_on_default_construction() {
    Vector<int> v;
    CHK(v.empty());
}

// Verifies empty() returns false after inserting an element.
static void not_empty_after_push_back() {
    Vector<int> v;
    v.push_back(1);
    CHK(!v.empty());
}

// Verifies empty() returns true after clearing the vector.
static void empty_after_clear() {
    Vector<int> v{1, 2, 3};
    v.clear();
    CHK(v.empty());
}

// Verifies size() tracks the number of inserted elements.
static void size_tracks_elements() {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    CHK(v.size() == 3);
}

// Verifies size() decreases after removing the last element.
static void size_decreases_on_pop_back() {
    Vector<int> v{1, 2, 3};
    v.pop_back();
    CHK(v.size() == 2);
}

// Verifies a default-constructed vector has zero capacity.
static void capacity_zero_on_default_construction() {
    Vector<int> v;
    CHK(v.capacity() == 0);
}

// Verifies capacity() is never smaller than size().
static void capacity_at_least_size() {
    Vector<int> v{1, 2, 3};
    CHK(v.capacity() >= v.size());
}

// Verifies capacity() grows as elements are appended.
static void capacity_grows_on_overflow() {
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

    CHK(grew);
}

// Verifies pop_back() does not reduce capacity().
static void capacity_unchanged_by_pop_back() {
    Vector<int> v{1, 2, 3};
    std::size_t capBefore = v.capacity();
    v.pop_back();

    CHK(v.capacity() == capBefore);
}

// Verifies capacity growth meets the expected expansion policy.
static void capacity_growth_ratio_respected() {
    Vector<int> v;
    v.reserve(10);
    std::size_t capBefore = v.capacity();

    for (std::size_t i = 0; i < capBefore; ++i)
        v.push_back(0);

    v.push_back(0); // Forces growth.

    CHK(v.capacity() >= capBefore * 2);
}

// Executes all capacity test cases.
static void run_tests() {
    RUN(empty_on_default_construction);
    RUN(not_empty_after_push_back);
    RUN(empty_after_clear);
    RUN(size_tracks_elements);
    RUN(size_decreases_on_pop_back);
    RUN(capacity_zero_on_default_construction);
    RUN(capacity_at_least_size);
    RUN(capacity_grows_on_overflow);
    RUN(capacity_unchanged_by_pop_back);
    RUN(capacity_growth_ratio_respected);
}

REGISTER_TEST_SUITE();