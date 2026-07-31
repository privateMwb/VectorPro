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

#include <support/framework.h>

using namespace VectorPro;

// Verifies begin() and end() delimit the full element range.
static void begin_end_basic() {
    Vector<int> v{1, 2, 3};
    auto it = v.begin();

    CHK(*it == 1);
    CHK(v.end() - v.begin() == 3);
}

// Verifies begin() equals end() for an empty vector.
static void begin_equals_end_when_empty() {
    Vector<int> v;
    CHK(v.begin() == v.end());
}

// Verifies range-based for iteration visits every element.
static void range_for_iterates_all_elements() {
    Vector<int> v{1, 2, 3};
    int sum = 0;
    for (int x : v)
        sum += x;

    CHK(sum == 6);
}

// Verifies mutable iterators provide writable element access.
static void iterator_mutates_elements() {
    Vector<int> v{1, 2, 3};
    for (auto it = v.begin(); it != v.end(); ++it)
        *it *= 10;

    CHK(v[0] == 10);
    CHK(v[1] == 20);
    CHK(v[2] == 30);
}

// Verifies const begin() and end() provide read-only iteration.
static void const_begin_end() {
    const Vector<int> v{1, 2, 3};
    CHK(*v.begin() == 1);
    CHK(v.end() - v.begin() == 3);
}

// Verifies cbegin() and cend() provide const iterators.
static void cbegin_cend_basic() {
    Vector<int> v{1, 2, 3};
    auto it = v.cbegin();

    CHK(*it == 1);
    CHK(v.cend() - v.cbegin() == 3);
}

// Verifies reverse iteration starts from the last element.
static void rbegin_rend_order() {
    Vector<int> v{1, 2, 3};
    auto it = v.rbegin();

    CHK(*it == 3);
    ++it;
    CHK(*it == 2);
}

// Verifies reverse iteration visits every element in reverse order.
static void rbegin_rend_full_traversal() {
    Vector<int> v{1, 2, 3};
    int expected = 3;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        CHK(*it == expected);
        --expected;
    }
}

// Verifies crbegin() and crend() provide const reverse iterators.
static void crbegin_crend_basic() {
    const Vector<int> v{1, 2, 3};
    auto it = v.crbegin();

    CHK(*it == 3);
    CHK(v.crend() - v.crbegin() == 3);
}

// Verifies iterator converts implicitly to const_iterator.
static void iterator_to_const_iterator_conversion() {
    Vector<int> v{1, 2, 3};
    Vector<int>::iterator it = v.begin();
    Vector<int>::const_iterator cit = it;

    CHK(*cit == 1);
}

// Verifies iterator arithmetic supports random-access traversal.
static void iterator_arithmetic() {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.begin() + 2;

    CHK(*it == 3);
    it -= 1;
    CHK(*it == 2);
}

// Verifies iterators support indexed random-access operations.
static void iterator_random_access_offset() {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.begin();

    CHK(it[3] == 4);
}

// Verifies iterator comparison operators behave correctly.
static void iterator_comparison() {
    Vector<int> v{1, 2, 3};
    auto a = v.begin();
    auto b = v.begin() + 1;

    CHK(a < b);
    CHK(b > a);
    CHK(a != b);
}

// Verifies the iterator types satisfy the contiguous_iterator concept.
static void satisfies_contiguous_iterator_concept() {
    static_assert(std::contiguous_iterator<Vector<int>::iterator>);
    static_assert(std::contiguous_iterator<Vector<int>::const_iterator>);
    CHK(true);
}

// Executes all iterator test cases.
static void run_tests() {
    RUN(begin_end_basic);
    RUN(begin_equals_end_when_empty);
    RUN(range_for_iterates_all_elements);
    RUN(iterator_mutates_elements);
    RUN(const_begin_end);
    RUN(cbegin_cend_basic);
    RUN(rbegin_rend_order);
    RUN(rbegin_rend_full_traversal);
    RUN(crbegin_crend_basic);
    RUN(iterator_to_const_iterator_conversion);
    RUN(iterator_arithmetic);
    RUN(iterator_random_access_offset);
    RUN(iterator_comparison);
    RUN(satisfies_contiguous_iterator_concept);
}

REGISTER_TEST_SUITE();