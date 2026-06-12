// VectorPro Examples
// Demonstrates basic usage of the dynamic array container:
//
// - basic usage (push_back, operator[], size, capacity)
// - emplace_back (in-place construction)
// - insert and erase (front, middle, back)
// - remove_if (predicate-based filtering)
// - iterators (forward and reverse traversal)
// - copy and move semantics
// - reserve and shrink_to_fit
// - element access (at, front, back, data_ptr)
// - observer (subscribe, event firing, unsubscribe)
//
// These examples illustrate the core features and intended usage of VectorPro.

#include "VectorPro.h"

// Basic Usage
// shows push_back, operator[], size, and capacity
void basicUsage() {
    VectorPro<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    (void)v.size();     // 3
    (void)v.capacity(); // >= 3
    (void)v[0];         // 1
    (void)v[1];         // 2
    (void)v[2];         // 3
}

// Emplace Back
// shows in-place construction of a struct directly inside the vector
void emplaceBack() {
    struct Point {
        int x, y;
        Point(int x, int y) : x(x), y(y) {}
    };

    VectorPro<Point> v;

    v.emplace_back(1, 2);
    v.emplace_back(3, 4);
    v.emplace_back(5, 6);

    (void)v[0].x; // 1
    (void)v[1].y; // 4
    (void)v[2].x; // 5
}

// Insert and Erase
// shows insertion at front, middle, and back, and removal by index
void insertErase() {
    VectorPro<int> v{1, 2, 3, 4, 5};

    // Insert at front
    v.insert(0, 0);
    (void)v[0]; // 0

    // Insert at middle
    v.insert(v.size() / 2, 99);

    // Insert at back
    v.insert(v.size(), 100);
    (void)v.back(); // 100

    // Erase front
    v.erase(0);
    (void)v[0]; // 1

    // Erase middle
    v.erase(v.size() / 2);

    // Erase back
    v.erase(v.size() - 1);
}

// Remove If
// shows predicate-based filtering with remove_if
void removeIf() {
    VectorPro<int> v{1, 2, 3, 4, 5, 6, 7, 8};

    // Remove all even numbers
    v.remove_if([](int x) { return x % 2 == 0; });

    (void)v.size(); // 4
    (void)v[0];     // 1
    (void)v[1];     // 3
}

// Iterators
// shows forward and reverse traversal
void iterators() {
    VectorPro<int> v{1, 2, 3, 4, 5};

    // Forward
    for (auto it = v.begin(); it != v.end(); ++it)
        (void)*it;

    // Const forward
    for (auto it = v.cbegin(); it != v.cend(); ++it)
        (void)*it;

    // Range-based for
    for (const auto& val : v)
        (void)val;

    // Reverse
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        (void)*it;

    // Const reverse
    for (auto it = v.crbegin(); it != v.crend(); ++it)
        (void)*it;
}

// Copy and Move
// shows copy construction, copy assignment, move construction, move assignment
void copyMove() {
    VectorPro<int> a{1, 2, 3};

    // Copy construction — b is an independent copy
    VectorPro<int> b(a);
    b[0] = 99;
    (void)a[0]; // 1 — unaffected

    // Copy assignment
    VectorPro<int> c;
    c = a;
    (void)c[0]; // 1

    // Move construction — a is emptied
    VectorPro<int> d(std::move(a));
    (void)d.size(); // 3
    (void)a.empty(); // true

    // Move assignment
    VectorPro<int> e;
    e = std::move(b);
    (void)e[0];      // 99
    (void)b.empty(); // true
}

// Reserve and Shrink
// shows reserve upfront and shrink_to_fit after removals
void reserveShrink() {
    VectorPro<int> v;

    // Reserve avoids repeated reallocation during bulk insert
    v.reserve(100);
    (void)v.capacity(); // >= 100
    (void)v.size();     // 0

    for (int i = 0; i < 10; ++i)
        v.push_back(i);

    // Shrink capacity down to match size
    v.shrink_to_fit();
    (void)v.capacity(); // 10
    (void)v.size();     // 10
}

// Element Access
// shows at(), front(), back(), operator[], and data_ptr()
void elementAccess() {
    VectorPro<int> v{10, 20, 30, 40, 50};

    (void)v.front();   // 10
    (void)v.back();    // 50
    (void)v[2];        // 30
    (void)v.at(2);     // 30

    // Raw pointer access
    int* raw = v.data_ptr();
    (void)raw[0]; // 10
    (void)raw[4]; // 50

    // Const overload
    const VectorPro<int>& cv  = v;
    const int*            craw = cv.data_ptr();
    (void)craw[0]; // 10
}

// Observer
// shows subscribe, event firing on mutations, and unsubscribe
void observer() {
    struct Logger {
        static void fn(const VectorPro<int>&, VectorPro<int>::EventType) {
            // fires on every mutation event
        }
    };

    VectorPro<int> v;
    v.subscribe(Logger::fn);

    v.push_back(1); // fires PushBack
    v.push_back(2); // fires PushBack
    v.pop_back();   // fires PopBack
    v.clear();      // fires Clear

    // unsubscribe by index — no further events fired
    v.unsubscribe(0);

    v.push_back(99); // silent — no listeners
}

// Entry Point
int main() {
    basicUsage();
    emplaceBack();
    insertErase();
    removeIf();
    iterators();
    copyMove();
    reserveShrink();
    elementAccess();
    observer();

    return 0;
}