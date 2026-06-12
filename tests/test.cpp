// VectorPro Unit Test Suite
// Tests correctness of core dynamic array operations:
//
// - constructor (initial state)
// - initializer list construction
// - push_back (append behavior and reallocation)
// - emplace_back (in-place construction)
// - pop_back (end removal and full drain)
// - insert (front, middle, back insertion and shifting)
// - erase (front, middle, back removal and shifting)
// - remove_if (predicate-based filtering)
// - clear (reset with capacity preservation)
// - reserve (capacity growth and no-op behavior)
// - shrink_to_fit (capacity reduction and edge cases)
// - front and back access and exception safety
// - element access (operator[] and at())
// - bounds checking (all throwing operations)
// - data_ptr (raw pointer access, const and non-const)
// - empty (state detection)
// - copy semantics (independence, mutation isolation, self-assignment)
// - move semantics (ownership transfer, source zeroed)
// - iterator traversal (forward, cbegin/cend, range-based for)
// - reverse iterator traversal (rbegin/rend, crbegin/crend)
// - observer (subscribe, event firing, unsubscribe)
//
// These tests validate correctness of VectorPro against expected std::vector-like behavior.

#include <iostream>
#include <cassert>
#include <string>

#include "VectorPro.h"

// Constructor Test
// verifies empty vector state on default construction
void constructor() {
    VectorPro<int> v;

    assert(v.size()     == 0);
    assert(v.capacity() == 0);
    assert(v.empty());

    std::cout << "\n[PASS] Constructor Test\n";
}

// Initializer List Test
// verifies correct values, size, and ordering from initializer list
void initializerList() {
    VectorPro<int> v{1, 2, 3, 4, 5};

    assert(v.size() == 5);
    assert(v[0]     == 1);
    assert(v[1]     == 2);
    assert(v[2]     == 3);
    assert(v[3]     == 4);
    assert(v[4]     == 5);

    std::cout << "\n[PASS] Initializer List Test\n";
}

// Push Back Test
// verifies append correctness, ordering, size growth, and reallocation
void pushBack() {
    VectorPro<int> v;

    for (int i = 0; i < 100; ++i)
        v.push_back(i);

    assert(v.size() == 100);

    for (int i = 0; i < 100; ++i)
        assert(v[i] == i);

    // rvalue push_back
    std::string s = "hello";
    VectorPro<std::string> sv;
    sv.push_back(std::move(s));

    assert(sv.size() == 1);
    assert(sv[0]     == "hello");

    std::cout << "\n[PASS] Push Back Test\n";
}

// Emplace Back Test
// verifies in-place construction and ordering
void emplaceBack() {
    struct Point {
        int x, y;
        Point(int x, int y) : x(x), y(y) {}
    };

    VectorPro<Point> v;

    v.emplace_back(1, 2);
    v.emplace_back(3, 4);
    v.emplace_back(5, 6);

    assert(v.size()  == 3);
    assert(v[0].x    == 1);
    assert(v[0].y    == 2);
    assert(v[1].x    == 3);
    assert(v[2].y    == 6);

    std::cout << "\n[PASS] Emplace Back Test\n";
}

// Pop Back Test
// verifies end removal, size decrement, and full drain
void popBack() {
    VectorPro<int> v{1, 2, 3, 4, 5};

    v.pop_back();

    assert(v.size() == 4);
    assert(v.back() == 4);

    v.pop_back();
    v.pop_back();

    assert(v.size() == 2);
    assert(v[0]     == 1);
    assert(v[1]     == 2);

    // full drain
    v.pop_back();
    v.pop_back();

    assert(v.size() == 0);
    assert(v.empty());

    // pop on empty is a no-op
    v.pop_back();
    assert(v.size() == 0);

    std::cout << "\n[PASS] Pop Back Test\n";
}

// Insert Test
// verifies front, middle, and back insertion with correct shifting
void insert() {
    VectorPro<int> v{1, 2, 3};

    // Insert at front
    v.insert(0, 0);

    assert(v.size() == 4);
    assert(v[0]     == 0);
    assert(v[1]     == 1);
    assert(v[2]     == 2);
    assert(v[3]     == 3);

    // Insert at middle
    v.insert(2, 99);

    assert(v.size() == 5);
    assert(v[1]     == 1);
    assert(v[2]     == 99);
    assert(v[3]     == 2);

    // Insert at back
    v.insert(v.size(), 100);

    assert(v.back() == 100);
    assert(v.size() == 6);

    std::cout << "\n[PASS] Insert Test\n";
}

// Erase Test
// verifies front, middle, and back removal with correct shifting
void erase() {
    VectorPro<int> v{1, 2, 3, 4, 5};

    // Erase front
    v.erase(0);

    assert(v.size() == 4);
    assert(v[0]     == 2);
    assert(v[1]     == 3);

    // Erase middle
    v.erase(1);

    assert(v.size() == 3);
    assert(v[0]     == 2);
    assert(v[1]     == 4);
    assert(v[2]     == 5);

    // Erase back
    v.erase(v.size() - 1);

    assert(v.size() == 2);
    assert(v.back() == 4);

    std::cout << "\n[PASS] Erase Test\n";
}

// Remove If Test
// verifies predicate filtering, remove all, and remove none
void removeIf() {
    // Remove evens
    {
        VectorPro<int> v{1, 2, 3, 4, 5, 6};

        v.remove_if([](int x) { return x % 2 == 0; });

        assert(v.size() == 3);
        assert(v[0]     == 1);
        assert(v[1]     == 3);
        assert(v[2]     == 5);
    }

    // Remove all
    {
        VectorPro<int> v{1, 2, 3};

        v.remove_if([](int) { return true; });

        assert(v.size() == 0);
        assert(v.empty());
    }

    // Remove none
    {
        VectorPro<int> v{1, 2, 3};

        v.remove_if([](int) { return false; });

        assert(v.size() == 3);
        assert(v[0]     == 1);
        assert(v[1]     == 2);
        assert(v[2]     == 3);
    }

    std::cout << "\n[PASS] Remove If Test\n";
}

// Clear Test
// verifies size resets to zero, capacity is preserved, and vector is reusable
void clear() {
    VectorPro<int> v{1, 2, 3, 4, 5};

    std::size_t capBefore = v.capacity();

    v.clear();

    assert(v.size()     == 0);
    assert(v.empty());
    assert(v.capacity() == capBefore);

    // reusable after clear
    v.push_back(42);

    assert(v.size() == 1);
    assert(v[0]     == 42);

    std::cout << "\n[PASS] Clear Test\n";
}

// Reserve Test
// verifies capacity growth, size unchanged, and no-op below current capacity
void reserve() {
    VectorPro<int> v;

    v.reserve(100);

    assert(v.capacity() >= 100);
    assert(v.size()      == 0);

    // no-op below current
    v.reserve(10);

    assert(v.capacity() >= 100);

    // elements still correct after reserve
    v.push_back(1);
    v.push_back(2);
    v.reserve(200);

    assert(v.size() == 2);
    assert(v[0]     == 1);
    assert(v[1]     == 2);

    std::cout << "\n[PASS] Reserve Test\n";
}

// Shrink To Fit Test
// verifies capacity matches size, no-op when already fit, empty vector case
void shrinkToFit() {
    // Normal shrink
    {
        VectorPro<int> v;
        v.reserve(100);

        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        v.shrink_to_fit();

        assert(v.size()     == 3);
        assert(v.capacity() == 3);
        assert(v[0]         == 1);
        assert(v[2]         == 3);
    }

    // Already fit — no-op
    {
        VectorPro<int> v;
        v.reserve(3);

        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        v.shrink_to_fit();

        assert(v.capacity() == 3);
    }

    // Empty vector — capacity goes to zero
    {
        VectorPro<int> v;
        v.reserve(64);
        v.shrink_to_fit();

        assert(v.capacity() == 0);
        assert(v.empty());
    }

    std::cout << "\n[PASS] Shrink To Fit Test\n";
}

// Front Back Test
// verifies correct values and exception on empty vector
void frontBack() {
    VectorPro<int> v{10, 20, 30};

    assert(v.front() == 10);
    assert(v.back()  == 30);

    v.push_back(40);

    assert(v.back() == 40);

    v.pop_back();
    v.pop_back();
    v.pop_back();
    v.pop_back();

    // front and back throw on empty
    bool frontThrown = false;
    bool backThrown  = false;

    try { (void)v.front(); } catch (const std::out_of_range&) { frontThrown = true; }
    try { (void)v.back();  } catch (const std::out_of_range&) { backThrown  = true; }

    assert(frontThrown);
    assert(backThrown);

    std::cout << "\n[PASS] Front Back Test\n";
}

// Element Access Test
// verifies operator[] and at() return correct values
void elementAccess() {
    VectorPro<int> v{5, 10, 15, 20, 25};

    assert(v[0]    == 5);
    assert(v[4]    == 25);
    assert(v.at(0) == 5);
    assert(v.at(4) == 25);

    // mutation via operator[]
    v[2] = 99;
    assert(v[2] == 99);

    // mutation via at()
    v.at(3) = 88;
    assert(v.at(3) == 88);

    std::cout << "\n[PASS] Element Access Test\n";
}

// Bounds Test
// verifies at(), front(), back(), insert(), erase() all throw on bad access
void bounds() {
    VectorPro<int> v{1, 2, 3};

    bool atThrown     = false;
    bool insertThrown = false;
    bool eraseThrown  = false;

    try { (void)v.at(10);      } catch (const std::out_of_range&) { atThrown     = true; }
    try { v.insert(99, 0);     } catch (const std::out_of_range&) { insertThrown = true; }
    try { v.erase(99);         } catch (const std::out_of_range&) { eraseThrown  = true; }

    assert(atThrown);
    assert(insertThrown);
    assert(eraseThrown);

    // front and back throw on empty
    VectorPro<int> empty;

    bool frontThrown = false;
    bool backThrown  = false;

    try { (void)empty.front(); } catch (const std::out_of_range&) { frontThrown = true; }
    try { (void)empty.back();  } catch (const std::out_of_range&) { backThrown  = true; }

    assert(frontThrown);
    assert(backThrown);

    std::cout << "\n[PASS] Bounds Test\n";
}

// Data Ptr Test
// verifies non-const and const raw pointer access
void dataPtr() {
    VectorPro<int> v{1, 2, 3};

    int* raw = v.data_ptr();

    assert(raw    != nullptr);
    assert(raw[0] == 1);
    assert(raw[1] == 2);
    assert(raw[2] == 3);

    // mutation through raw pointer
    raw[0] = 99;
    assert(v[0] == 99);

    // const overload
    const VectorPro<int>& cv  = v;
    const int*             craw = cv.data_ptr();

    assert(craw    != nullptr);
    assert(craw[1] == 2);
    assert(craw[2] == 3);

    std::cout << "\n[PASS] Data Ptr Test\n";
}

// Empty Test
// verifies empty() state transitions correctly
void empty() {
    VectorPro<int> v;

    assert(v.empty());

    v.push_back(1);
    assert(!v.empty());

    v.push_back(2);
    assert(!v.empty());

    v.pop_back();
    v.pop_back();
    assert(v.empty());

    v.push_back(42);
    v.clear();
    assert(v.empty());

    std::cout << "\n[PASS] Empty Test\n";
}

// Copy Test
// verifies independent copies, mutation isolation, and self-assignment safety
void copy() {
    // Copy construction
    {
        VectorPro<int> a{1, 2, 3};
        VectorPro<int> b(a);

        assert(b.size() == 3);
        assert(b[0]     == 1);
        assert(b[1]     == 2);
        assert(b[2]     == 3);

        // mutating b does not affect a
        b[0] = 99;

        assert(a[0] == 1);
        assert(b[0] == 99);
    }

    // Copy assignment
    {
        VectorPro<int> a{4, 5, 6};
        VectorPro<int> b{7, 8};

        b = a;

        assert(b.size() == 3);
        assert(b[0]     == 4);

        // mutating b does not affect a
        b[1] = 99;

        assert(a[1] == 5);
        assert(b[1] == 99);
    }

    // Self-assignment
    {
        VectorPro<int> a{1, 2, 3};

        a = a;

        assert(a.size() == 3);
        assert(a[0]     == 1);
        assert(a[1]     == 2);
        assert(a[2]     == 3);
    }

    std::cout << "\n[PASS] Copy Test\n";
}

// Move Test
// verifies ownership transfer and source zeroed after move
void move() {
    // Move construction
    {
        VectorPro<int> a{1, 2, 3};
        VectorPro<int> b(std::move(a));

        assert(b.size()  == 3);
        assert(b[0]      == 1);
        assert(b[1]      == 2);
        assert(b[2]      == 3);
        assert(a.size()  == 0);
        assert(a.empty());
    }

    // Move assignment
    {
        VectorPro<int> a{4, 5, 6};
        VectorPro<int> b{7, 8};

        b = std::move(a);

        assert(b.size() == 3);
        assert(b[0]     == 4);
        assert(a.size() == 0);
        assert(a.empty());
    }

    std::cout << "\n[PASS] Move Test\n";
}

// Iterator Test
// verifies forward traversal, cbegin/cend, and range-based for
void iterator() {
    VectorPro<int> v{1, 2, 3, 4, 5};

    // Manual forward traversal
    int expected = 1;

    for (auto it = v.begin(); it != v.end(); ++it) {
        assert(*it == expected);
        ++expected;
    }

    // cbegin / cend
    expected = 1;

    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        assert(*it == expected);
        ++expected;
    }

    // Range-based for
    expected = 1;

    for (const auto& val : v) {
        assert(val == expected);
        ++expected;
    }

    std::cout << "\n[PASS] Iterator Test\n";
}

// Reverse Iterator Test
// verifies rbegin/rend and crbegin/crend traversal
void reverseIterator() {
    VectorPro<int> v{1, 2, 3, 4, 5};

    // rbegin / rend
    int expected = 5;

    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        assert(*it == expected);
        --expected;
    }

    // crbegin / crend
    expected = 5;

    for (auto it = v.crbegin(); it != v.crend(); ++it) {
        assert(*it == expected);
        --expected;
    }

    std::cout << "\n[PASS] Reverse Iterator Test\n";
}

// Observer Test
// verifies subscribe fires on events, correct count, and unsubscribe stops firing
void observer() {
    static int fireCount = 0;
    fireCount = 0;

    struct Counter {
        static void fn(const VectorPro<int>&, VectorPro<int>::EventType) {
            ++fireCount;
        }
    };

    VectorPro<int> v;
    v.subscribe(Counter::fn);

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    assert(fireCount == 3);

    v.pop_back();

    assert(fireCount == 4);

    v.clear();

    assert(fireCount == 5);

    // unsubscribe stops firing
    v.unsubscribe(0);

    v.push_back(99);
    v.push_back(100);

    assert(fireCount == 5);

    // unsubscribe out of range throws
    bool thrown = false;

    try { v.unsubscribe(99); } catch (const std::out_of_range&) { thrown = true; }

    assert(thrown);

    std::cout << "\n[PASS] Observer Test\n";
}

// Entry Point
int main() {
    constructor();
    initializerList();
    pushBack();
    emplaceBack();
    popBack();
    insert();
    erase();
    removeIf();
    clear();
    reserve();
    shrinkToFit();
    frontBack();
    elementAccess();
    bounds();
    dataPtr();
    empty();
    copy();
    move();
    iterator();
    reverseIterator();
    observer();

    return 0;
}


