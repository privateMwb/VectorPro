// Vector Access Test Suite
// Verifies element accessors and view operations.
//
// Covers:
// - as_span
// - data_ptr
// - at
// - front and back
// - operator[]
// - const accessors

#include <support/framework.h>

using namespace VectorPro;

// Verifies as_span() exposes the vector contents as a contiguous view.
static void as_span_basic() {
    Vector<int> v{1, 2, 3};
    auto span = v.as_span();

    CHK(span.size() == 3);
    CHK(span[0] == 1);
    CHK(span[2] == 3);
}

// Verifies modifications through as_span() are reflected in the vector.
static void as_span_reflects_mutation() {
    Vector<int> v{1, 2, 3};
    auto span = v.as_span();
    span[0] = 99;

    CHK(v[0] == 99);
}

// Verifies the const overload of as_span() provides read-only access.
static void as_span_const() {
    const Vector<int> v{1, 2, 3};
    auto span = v.as_span();

    CHK(span.size() == 3);
    CHK(span[1] == 2);
}

// Verifies data_ptr() returns a pointer to the underlying storage.
static void data_ptr_basic() {
    Vector<int> v{1, 2, 3};
    int* p = v.data_ptr();

    CHK(p[0] == 1);
    CHK(p == &v[0]);
}

// Verifies data_ptr() returns nullptr for an empty vector.
static void data_ptr_null_when_empty() {
    Vector<int> v;
    CHK(v.data_ptr() == nullptr);
}

// Verifies at() returns the requested element for a valid index.
static void at_valid_index() {
    Vector<int> v{1, 2, 3};
    CHK(v.at(1) == 2);
}

// Verifies at() throws for an out-of-range index.
static void at_out_of_range_throws() {
    Vector<int> v{1, 2, 3};
    bool threw = false;
    try {
        (void)v.at(10);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    CHK(threw);
}

// Verifies front() returns the first element.
static void front_basic() {
    Vector<int> v{5, 6, 7};
    CHK(v.front() == 5);
}

// Verifies back() returns the last element.
static void back_basic() {
    Vector<int> v{5, 6, 7};
    CHK(v.back() == 7);
}

// Verifies front() and back() return the same element for a single-element vector.
static void front_back_single_element() {
    Vector<int> v{42};
    CHK(v.front() == 42);
    CHK(v.back() == 42);
}

// Verifies operator[] provides read access to elements.
static void operator_index_read() {
    Vector<int> v{1, 2, 3};
    CHK(v[0] == 1);
    CHK(v[2] == 3);
}

// Verifies operator[] provides writable access to elements.
static void operator_index_write() {
    Vector<int> v{1, 2, 3};
    v[1] = 99;
    CHK(v[1] == 99);
}

// Verifies all const accessors return the expected values.
static void const_access() {
    const Vector<int> v{1, 2, 3};
    CHK(v[0] == 1);
    CHK(v.front() == 1);
    CHK(v.back() == 3);
    CHK(v.at(1) == 2);
}

// Executes all access test cases.
static void run_tests() {
    RUN(as_span_basic);
    RUN(as_span_reflects_mutation);
    RUN(as_span_const);
    RUN(data_ptr_basic);
    RUN(data_ptr_null_when_empty);
    RUN(at_valid_index);
    RUN(at_out_of_range_throws);
    RUN(front_basic);
    RUN(back_basic);
    RUN(front_back_single_element);
    RUN(operator_index_read);
    RUN(operator_index_write);
    RUN(const_access);
}

REGISTER_TEST_SUITE();