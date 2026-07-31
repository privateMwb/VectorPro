// Vector Modifiers Test Suite
// Verifies element insertion, removal, and capacity management.
//
// Covers:
// - push_back with lvalues
// - push_back with rvalues
// - push_back triggering reallocation
// - push_back with self-referenced elements
// - emplace_back
// - insert at the beginning
// - insert at the middle
// - insert at the end
// - insert with rvalues
// - insert with self-referenced elements
// - range insertion
// - emplace at arbitrary positions
// - remove_if with partial, no, and full matches
// - pop_back on populated and empty vectors
// - single-element erase
// - range erase
// - clear on populated and empty vectors
// - reserve growing capacity
// - reserve with a smaller requested capacity
// - shrink_to_fit

#include <support/framework.h>

using namespace VectorPro;

// Verifies that push_back(const T&) appends and grows size correctly.
static void push_back_lvalue() {
    Vector<int> v;
    int x = 5;
    v.push_back(x);
    v.push_back(x);

    CHK(v.size() == 2);
    CHK(v[0] == 5);
    CHK(v[1] == 5);
}

// Verifies that push_back(T&&) moves the value in.
static void push_back_rvalue() {
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);

    CHK(v.size() == 2);
    CHK(v[0] == 10);
    CHK(v[1] == 20);
}

// Verifies that repeated push_back triggers capacity growth without data loss.
static void push_back_growth() {
    Vector<int> v;
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    CHK(v.size() == 100);
    CHK(v.capacity() >= 100);
    for (int i = 0; i < 100; ++i) {
        CHK(v[i] == i);
    }
}

// Verifies that push_back(v[i]) survives a reallocation triggered by the call.
static void push_back_self_reference() {
    Vector<int> v(4, 0);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    v[3] = 4;
    // size == capacity, so this call forces reallocate() while `value`
    // still references the old (about-to-be-freed) buffer.
    v.push_back(v[0]);

    CHK(v.size() == 5);
    CHK(v[4] == 1);
    CHK(v[0] == 1);
}

// Verifies that emplace_back constructs the element in place with forwarded args.
static void emplace_back_basic() {
    Vector<int> v;
    v.emplace_back(42);
    v.emplace_back(43);

    CHK(v.size() == 2);
    CHK(v[0] == 42);
    CHK(v[1] == 43);
}

// Verifies that insert(pos, value) at the end behaves like push_back.
static void insert_at_end() {
    Vector<int> v{1, 2, 3};
    auto it = v.insert(v.cend(), 4);

    CHK(v.size() == 4);
    CHK(v[3] == 4);
    CHK(*it == 4);
}

// Verifies that insert(pos, value) at the beginning shifts existing elements.
static void insert_at_beginning() {
    Vector<int> v{1, 2, 3};
    auto it = v.insert(v.cbegin(), 0);

    CHK(v.size() == 4);
    CHK(v[0] == 0);
    CHK(v[1] == 1);
    CHK(v[2] == 2);
    CHK(v[3] == 3);
    CHK(*it == 0);
}

// Verifies that insert(pos, value) in the middle shifts only the tail.
static void insert_at_middle() {
    Vector<int> v{1, 2, 4, 5};
    auto it = v.insert(v.cbegin() + 2, 3);

    CHK(v.size() == 5);
    CHK(v[0] == 1);
    CHK(v[1] == 2);
    CHK(v[2] == 3);
    CHK(v[3] == 4);
    CHK(v[4] == 5);
    CHK(*it == 3);
}

// Verifies that insert(pos, T&&) moves the value into place.
static void insert_rvalue() {
    Vector<int> v{1, 3};
    (void)v.insert(v.cbegin() + 1, 2);

    CHK(v.size() == 3);
    CHK(v[1] == 2);
}

// Verifies that insert(pos, v[i]) survives reallocation and shifting alike.
static void insert_self_reference() {
    Vector<int> v(4, 0);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    v[3] = 4;
    // size == capacity: forces reallocate(); value also aliases the source buffer.
    (void)v.insert(v.cbegin() + 1, v[3]);

    CHK(v.size() == 5);
    CHK(v[1] == 4);
}

// Verifies that insert(pos, first, last) inserts a full range at the given position.
static void insert_range() {
    Vector<int> v{1, 2, 6};
    int src[] = {3, 4, 5};
    v.insert(v.cbegin() + 2, std::begin(src), std::end(src));

    CHK(v.size() == 6);
    CHK(v[0] == 1);
    CHK(v[1] == 2);
    CHK(v[2] == 3);
    CHK(v[3] == 4);
    CHK(v[4] == 5);
    CHK(v[5] == 6);
}

// Verifies that emplace(pos, args...) constructs in place at the given index.
static void emplace_at_middle() {
    Vector<int> v{1, 2, 4};
    auto it = v.emplace(v.cbegin() + 2, 3);

    CHK(v.size() == 4);
    CHK(v[2] == 3);
    CHK(v[3] == 4);
    CHK(*it == 3);
}

// Verifies that emplace(pos, args...) at the end constructs directly without shifting.
static void emplace_at_end() {
    Vector<int> v{1, 2};
    (void)v.emplace(v.cend(), 3);

    CHK(v.size() == 3);
    CHK(v[2] == 3);
}

// Verifies that remove_if compacts the vector and returns the removed count.
static void remove_if_basic() {
    Vector<int> v{1, 2, 3, 4, 5, 6};
    std::size_t removed = v.remove_if([](const int& x) { return x % 2 == 0; });

    CHK(removed == 3);
    CHK(v.size() == 3);
    CHK(v[0] == 1);
    CHK(v[1] == 3);
    CHK(v[2] == 5);
}

// Verifies that remove_if with no matches leaves the vector unchanged.
static void remove_if_no_match() {
    Vector<int> v{1, 3, 5};
    std::size_t removed = v.remove_if([](const int& x) { return x % 2 == 0; });

    CHK(removed == 0);
    CHK(v.size() == 3);
}

// Verifies that remove_if matching everything empties the vector.
static void remove_if_all_match() {
    Vector<int> v{2, 4, 6};
    std::size_t removed = v.remove_if([](const int&) { return true; });

    CHK(removed == 3);
    CHK(v.size() == 0);
}

// Verifies that pop_back removes the last element and shrinks size by one.
static void pop_back_basic() {
    Vector<int> v{1, 2, 3};
    v.pop_back();

    CHK(v.size() == 2);
    CHK(v[0] == 1);
    CHK(v[1] == 2);
}

// Verifies that pop_back on an empty vector is a safe no-op.
static void pop_back_empty() {
    Vector<int> v;
    v.pop_back();

    CHK(v.size() == 0);
}

// Verifies that erase(pos) removes a single element and shifts the tail down.
static void erase_single() {
    Vector<int> v{1, 2, 3, 4};
    auto it = v.erase(v.cbegin() + 1);

    CHK(v.size() == 3);
    CHK(v[0] == 1);
    CHK(v[1] == 3);
    CHK(v[2] == 4);
    CHK(*it == 3);
}

// Verifies that erase(pos) on the last element leaves the tail iterator at end().
static void erase_last_element() {
    Vector<int> v{1, 2, 3};
    auto it = v.erase(v.cbegin() + 2);

    CHK(v.size() == 2);
    CHK(it == v.end());
}

// Verifies that erase(first, last) removes a contiguous range.
static void erase_range() {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.erase(v.cbegin() + 1, v.cbegin() + 4);

    CHK(v.size() == 2);
    CHK(v[0] == 1);
    CHK(v[1] == 5);
    CHK(*it == 5);
}

// Verifies that erase(first, last) with an empty range is a safe no-op.
static void erase_empty_range() {
    Vector<int> v{1, 2, 3};
    auto it = v.erase(v.cbegin() + 1, v.cbegin() + 1);

    CHK(v.size() == 3);
    CHK(*it == 2);
}

// Verifies that clear destroys all elements and resets size to zero.
static void clear_basic() {
    Vector<int> v{1, 2, 3};
    std::size_t capBefore = v.capacity();
    v.clear();

    CHK(v.size() == 0);
    CHK(v.capacity() == capBefore);
    CHK(v.empty() == true);
}

// Verifies that clear on an already-empty vector is a safe no-op.
static void clear_empty() {
    Vector<int> v;
    v.clear();

    CHK(v.size() == 0);
}

// Verifies that reserve grows capacity without changing size or contents.
static void reserve_grows_capacity() {
    Vector<int> v{1, 2, 3};
    v.reserve(50);

    CHK(v.capacity() >= 50);
    CHK(v.size() == 3);
    CHK(v[0] == 1);
    CHK(v[1] == 2);
    CHK(v[2] == 3);
}

// Verifies that reserve with a smaller-than-current capacity is a no-op.
static void reserve_smaller_is_noop() {
    Vector<int> v{1, 2, 3};
    v.reserve(50);
    std::size_t capAfterFirst = v.capacity();
    v.reserve(1);

    CHK(v.capacity() == capAfterFirst);
}

// Verifies that shrink_to_fit reduces capacity to match size.
static void shrink_to_fit_basic() {
    Vector<int> v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.shrink_to_fit();

    CHK(v.capacity() == v.size());
    CHK(v[0] == 1);
    CHK(v[1] == 2);
}

// Verifies that shrink_to_fit when already tight is a safe no-op.
static void shrink_to_fit_already_tight() {
    Vector<int> v{1, 2, 3};
    v.shrink_to_fit();

    CHK(v.capacity() == v.size());
    CHK(v.size() == 3);
}

// Executes all modifier test cases.
static void run_tests() {
    RUN(push_back_lvalue);
    RUN(push_back_rvalue);
    RUN(push_back_growth);
    RUN(push_back_self_reference);
    RUN(emplace_back_basic);
    RUN(insert_at_end);
    RUN(insert_at_beginning);
    RUN(insert_at_middle);
    RUN(insert_rvalue);
    RUN(insert_self_reference);
    RUN(insert_range);
    RUN(emplace_at_middle);
    RUN(emplace_at_end);
    RUN(remove_if_basic);
    RUN(remove_if_no_match);
    RUN(remove_if_all_match);
    RUN(pop_back_basic);
    RUN(pop_back_empty);
    RUN(erase_single);
    RUN(erase_last_element);
    RUN(erase_range);
    RUN(erase_empty_range);
    RUN(clear_basic);
    RUN(clear_empty);
    RUN(reserve_grows_capacity);
    RUN(reserve_smaller_is_noop);
    RUN(shrink_to_fit_basic);
    RUN(shrink_to_fit_already_tight);
}

REGISTER_TEST_SUITE();