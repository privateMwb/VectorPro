// Vector Modifier Test Suite
// Validates all mutating operations on the vector including
// insertion, removal, emplacement, and capacity management.
//
// Covers:
// - push_back (copy and move)
// - emplace_back
// - insert (single, move, range)
// - emplace
// - remove_if
// - pop_back
// - erase (single and range)
// - clear
// - reserve
// - shrink_to_fit

#include "test_helper.h"
#include "Vector.h"

using namespace VectorPro;

// Push Back Copy
// Verifies elements are correctly appended via copy.
static void push_back_copy() {
    Vector<int> v;
    int val = 42;
    v.push_back(val);
    CHK(v.size() == 1);
    CHK(v[0]     == 42);
}

// Push Back Move
// Verifies elements are correctly appended via move.
static void push_back_move() {
    Vector<std::string> v;
    std::string s = "hello";
    v.push_back(std::move(s));
    CHK(v.size() == 1);
    CHK(v[0]     == "hello");
    CHK(s.empty());
}

// Emplace Back
// Verifies in-place construction at the end.
static void emplace_back() {
    Vector<std::string> v;
    v.emplace_back(3, 'x');
    CHK(v.size() == 1);
    CHK(v[0]     == "xxx");
}

// Insert Copy
// Verifies element is inserted at the correct position.
static void insert_copy() {
    Vector<int> v = { 1, 2, 4, 5 };
    int val = 3;
    (void)v.insert(v.cbegin() + 2, val);
    CHK(v.size() == 5);
    CHK(v[2]     == 3);
    CHK(v[3]     == 4);
}

// Insert Move
// Verifies element is moved into the correct position.
static void insert_move() {
    Vector<std::string> v = { "a", "c" };
    std::string s = "b";
    (void)v.insert(v.cbegin() + 1, std::move(s));
    CHK(v.size() == 3);
    CHK(v[1]     == "b");
    CHK(s.empty());
}

// Insert Range
// Verifies a range of elements is inserted at the correct position.
static void insert_range() {
    Vector<int> v   = { 1, 5 };
    Vector<int> src = { 2, 3, 4 };
    (void)v.insert(v.cbegin() + 1, src.begin(), src.end());
    CHK(v.size() == 5);
    CHK(v[1]     == 2);
    CHK(v[3]     == 4);
}

// Emplace
// Verifies in-place construction at a given position.
static void emplace() {
    Vector<std::string> v = { "a", "c" };
    (void)v.emplace(v.cbegin() + 1, 1, 'b');
    CHK(v.size() == 3);
    CHK(v[1]     == "b");
}

// Remove If
// Verifies elements matching a predicate are removed.
static void remove_if() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    std::size_t removed = v.remove_if([](const int& x) { return x % 2 == 0; });
    CHK(removed  == 2);
    CHK(v.size() == 3);
    CHK(v[0]     == 1);
    CHK(v[1]     == 3);
    CHK(v[2]     == 5);
}

// Pop Back
// Verifies the last element is removed correctly.
static void pop_back() {
    Vector<int> v = { 1, 2, 3 };
    v.pop_back();
    CHK(v.size() == 2);
    CHK(v[1]     == 2);
}

// Pop Back Empty
// Verifies pop_back on an empty vector is a no-op.
static void pop_back_empty() {
    Vector<int> v;
    v.pop_back();
    CHK(v.size() == 0);
    CHK(v.empty());
}

// Erase Single
// Verifies a single element is erased at the correct position.
static void erase_single() {
    Vector<int> v = { 1, 2, 3, 4 };
    (void)v.erase(v.cbegin() + 1);
    CHK(v.size() == 3);
    CHK(v[1]     == 3);
}

// Erase Range
// Verifies a range of elements is erased correctly.
static void erase_range() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    (void)v.erase(v.cbegin() + 1, v.cbegin() + 4);
    CHK(v.size() == 2);
    CHK(v[0]     == 1);
    CHK(v[1]     == 5);
}

// Clear
// Verifies all elements are removed and size is reset to zero.
static void clear() {
    Vector<int> v = { 1, 2, 3 };
    v.clear();
    CHK(v.size()  == 0);
    CHK(v.empty());
}

// Clear Empty
// Verifies clear on an empty vector is a no-op.
static void clear_empty() {
    Vector<int> v;
    v.clear();
    CHK(v.size()  == 0);
    CHK(v.empty());
}

// Reserve
// Verifies capacity is increased without affecting size.
static void reserve() {
    Vector<int> v;
    v.reserve(20);
    CHK(v.size()     == 0);
    CHK(v.capacity() >= 20);
}

// Reserve Below Capacity
// Verifies reserve does nothing when newCap is less than current capacity.
static void reserve_below_capacity() {
    Vector<int> v;
    v.reserve(20);
    v.reserve(5);
    CHK(v.capacity() >= 20);
}

// Shrink To Fit
// Verifies capacity is reduced to match size.
static void shrink_to_fit() {
    Vector<int> v = { 1, 2, 3 };
    v.reserve(20);
    v.shrink_to_fit();
    CHK(v.capacity() == v.size());
}

// Test Runner
// Executes all modifier test cases.
void run_modifier_tests() {
    setTitle("Modifier Tests");

    RUN(push_back_copy);
    RUN(push_back_move);
    RUN(emplace_back);
    RUN(insert_copy);
    RUN(insert_move);
    RUN(insert_range);
    RUN(emplace);
    RUN(remove_if);
    RUN(pop_back);
    RUN(pop_back_empty);
    RUN(erase_single);
    RUN(erase_range);
    RUN(clear);
    RUN(clear_empty);
    RUN(reserve);
    RUN(reserve_below_capacity);
    RUN(shrink_to_fit);

    std::cout << "\n";
}