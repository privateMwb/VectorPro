// Vector Constructor Test Suite
// Validates construction, assignment, allocator support,
// ownership transfer, and growth policy behavior.
//
// Covers:
// - Default construction
// - Fill construction
// - Initializer-list construction
// - Iterator-range construction
// - Copy construction
// - Move construction
// - Copy assignment
// - Move assignment
// - Custom growth factors
// - Custom allocator support

#include "test_helper.h"
#include "Vector.h"

using namespace VectorPro;

// Default Constructor
// Ensures a new vector starts empty with zero capacity.
static void default_constructor() {
    Vector<int> v;
    CHK(v.size()     == 0);
    CHK(v.capacity() == 0);
    CHK(v.empty());
}

// Fill Constructor
// Verifies size, capacity, and value initialization.
static void fill_constructor() {
    Vector<int> v(5, 42);
    CHK(v.size()     == 5);
    CHK(v.capacity() == 5);
    CHK(!v.empty());

    for (std::size_t i = 0; i < v.size(); ++i)
        CHK(v[i] == 42);
}

// Initializer List Constructor
// Ensures elements are copied from an initializer list.
static void initializer_list_constructor() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    CHK(v.size() == 5);
    CHK(v[0]     == 1);
    CHK(v[4]     == 5);
}

// Iterator Range Constructor
// Verifies construction from a pair of iterators.
static void iterator_range_constructor() {
    Vector<int> src = { 10, 20, 30 };
    Vector<int> v(src.begin(), src.end());
    CHK(v.size() == 3);
    CHK(v[0]     == 10);
    CHK(v[2]     == 30);
}

// Copy Constructor
// Ensures deep-copy semantics and independent storage.
static void copy_constructor() {
    Vector<int> src = { 1, 2, 3 };
    Vector<int> v(src);
    CHK(v.size() == src.size());
    CHK(v[0]     == src[0]);
    CHK(v[2]     == src[2]);

    // ensure deep copy
    v[0] = 99;
    CHK(src[0] == 1);
}

// Move Constructor
// Verifies ownership transfer from the source vector.
static void move_constructor() {
    Vector<int> src = { 1, 2, 3 };
    Vector<int> v(std::move(src));
    CHK(v.size()   == 3);
    CHK(v[0]       == 1);
    CHK(src.size() == 0);
    CHK(src.empty());
}

// Copy Assignment
// Ensures assigned vectors receive independent copies.
static void copy_assignment() {
    Vector<int> src = { 7, 8, 9 };
    Vector<int> v;
    v = src;
    CHK(v.size() == 3);
    CHK(v[0]     == 7);

    // ensure deep copy
    v[0] = 99;
    CHK(src[0] == 7);
}

// Move Assignment
// Verifies resource transfer during assignment.
static void move_assignment() {
    Vector<int> src = { 4, 5, 6 };
    Vector<int> v;
    v = std::move(src);
    CHK(v.size()   == 3);
    CHK(v[0]       == 4);
    CHK(src.size() == 0);
}

// Custom Growth Factor
// Verifies capacity growth using a custom ratio.
static void custom_growth_factor() {
    Vector<int, std::allocator<int>, 3, 2> v;   // 1.5x growth
    for (int i = 0; i < 10; ++i)
        v.push_back(i);

    CHK(v.size() == 10);
    CHK(v[0]     == 0);
    CHK(v[9]     == 9);
}

// Custom Allocator
// Ensures allocation works with PMR allocators.
static void custom_allocator() {
    std::pmr::monotonic_buffer_resource pool;
    Vector<int, std::pmr::polymorphic_allocator<int>> v{
        std::pmr::polymorphic_allocator<int>(&pool)
    };

    for (int i = 0; i < 5; ++i)
        v.push_back(i);

    CHK(v.size() == 5);
    CHK(v[0]     == 0);
    CHK(v[4]     == 4);
}

// Test Runner
// Executes all constructor and assignment test cases.
void run_constructor_tests() {
    setTitle("Constructor Tests");
    
    RUN(default_constructor);
    RUN(fill_constructor);
    RUN(initializer_list_constructor);
    RUN(iterator_range_constructor);
    RUN(copy_constructor);
    RUN(move_constructor);
    RUN(copy_assignment);
    RUN(move_assignment);
    RUN(custom_growth_factor);
    RUN(custom_allocator);
    
    std::cout << "\n";
}