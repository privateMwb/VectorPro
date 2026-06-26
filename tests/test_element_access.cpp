// Vector Element Access & Search Test Suite
// Validates indexed access, bounds checking, convenience
// accessors, raw pointer access, and value search.
//
// Covers:
// - operator[] (read and write)
// - at() with valid index
// - at() throws on out of range
// - front() and back()
// - data_ptr()
// - as_span()
// - contains()
// - find()

#include "test_helper.h"
#include "Vector.h"

using namespace VectorPro;

// Operator Index Read
// Verifies elements can be read via operator[].
static void operator_index_read() {
    Vector<int> v = { 10, 20, 30 };
    CHK(v[0] == 10);
    CHK(v[1] == 20);
    CHK(v[2] == 30);
}

// Operator Index Write
// Verifies elements can be written via operator[].
static void operator_index_write() {
    Vector<int> v = { 1, 2, 3 };
    v[1] = 99;
    CHK(v[1] == 99);
}

// At Valid Index
// Verifies at() returns the correct element.
static void at_valid_index() {
    Vector<int> v = { 5, 10, 15 };
    CHK(v.at(0) == 5);
    CHK(v.at(2) == 15);
}

// At Out Of Range
// Verifies at() throws std::out_of_range for invalid index.
static void at_out_of_range() {
    Vector<int> v = { 1, 2, 3 };
    bool threw = false;
    try {
        (void)v.at(10);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    CHK(threw);
}

// Front
// Verifies front() returns the first element.
static void front() {
    Vector<int> v = { 7, 8, 9 };
    CHK(v.front() == 7);
}

// Front Write
// Verifies front() can be used to modify the first element.
static void front_write() {
    Vector<int> v = { 7, 8, 9 };
    v.front() = 99;
    CHK(v[0] == 99);
}

// Back
// Verifies back() returns the last element.
static void back() {
    Vector<int> v = { 7, 8, 9 };
    CHK(v.back() == 9);
}

// Back Write
// Verifies back() can be used to modify the last element.
static void back_write() {
    Vector<int> v = { 7, 8, 9 };
    v.back() = 99;
    CHK(v[2] == 99);
}

// Data Ptr
// Verifies data_ptr() returns a valid pointer to the first element.
static void data_ptr() {
    Vector<int> v = { 1, 2, 3 };
    int* ptr = v.data_ptr();
    CHK(ptr    != nullptr);
    CHK(ptr[0] == 1);
    CHK(ptr[2] == 3);
}

// As Span
// Verifies as_span() returns a valid view over the elements.
static void as_span() {
    Vector<int> v = { 1, 2, 3 };
    auto s = v.as_span();
    CHK(s.size() == 3);
    CHK(s[0]     == 1);
    CHK(s[2]     == 3);
}

// As Span Const
// Verifies const as_span() returns a read-only view.
static void as_span_const() {
    const Vector<int> v = { 4, 5, 6 };
    auto s = v.as_span();
    CHK(s.size() == 3);
    CHK(s[0]     == 4);
    CHK(s[2]     == 6);
}

// Contains True
// Verifies contains() returns true when the value exists.
static void contains_true() {
    Vector<int> v = { 1, 2, 3 };
    CHK(v.contains(2));
}

// Contains False
// Verifies contains() returns false when the value is absent.
static void contains_false() {
    Vector<int> v = { 1, 2, 3 };
    CHK(!v.contains(99));
}

// Find Existing
// Verifies find() returns an iterator to the matching element.
static void find_existing() {
    Vector<int> v = { 10, 20, 30 };
    auto it = v.find(20);
    CHK(it != v.end());
    CHK(*it == 20);
}

// Find Missing
// Verifies find() returns end() when the value is not found.
static void find_missing() {
    Vector<int> v = { 10, 20, 30 };
    auto it = v.find(99);
    CHK(it == v.end());
}

// Test Runner
// Executes all element access and search test cases.
void run_element_access_tests() {
    setTitle("Element Access & Search Tests");

    RUN(operator_index_read);
    RUN(operator_index_write);
    RUN(at_valid_index);
    RUN(at_out_of_range);
    RUN(front);
    RUN(front_write);
    RUN(back);
    RUN(back_write);
    RUN(data_ptr);
    RUN(as_span);
    RUN(as_span_const);
    RUN(contains_true);
    RUN(contains_false);
    RUN(find_existing);
    RUN(find_missing);

    std::cout << "\n";
}