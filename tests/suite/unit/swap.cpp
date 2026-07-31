// Vector Swap Test Suite
// Verifies member and non-member swap behavior.
//
// Covers:
// - member swap exchanges contents
// - member swap exchanges capacity
// - member swap with an empty vector
// - self member swap
// - free swap
// - free swap matches member swap
// - event listeners are transferred with swapped state

#include <support/framework.h>

using namespace VectorPro;

// Verifies member swap exchanges the contents of both vectors.
static void member_swap_exchanges_contents() {
    Vector<int> a{1, 2, 3};
    Vector<int> b{4, 5};

    a.swap(b);

    CHK(a.size() == 2);
    CHK(a[0] == 4);
    CHK(a[1] == 5);
    CHK(b.size() == 3);
    CHK(b[0] == 1);
    CHK(b[1] == 2);
    CHK(b[2] == 3);
}

// Verifies member swap exchanges the capacities of both vectors.
static void member_swap_exchanges_capacity() {
    Vector<int> a;
    a.reserve(100);
    Vector<int> b;
    b.reserve(10);

    std::size_t capA = a.capacity();
    std::size_t capB = b.capacity();

    a.swap(b);

    CHK(a.capacity() == capB);
    CHK(b.capacity() == capA);
}

// Verifies member swap correctly exchanges a populated vector with an empty one.
static void member_swap_with_empty() {
    Vector<int> a{1, 2, 3};
    Vector<int> b;

    a.swap(b);

    CHK(a.size() == 0);
    CHK(b.size() == 3);
    CHK(b[0] == 1);
}

// Verifies self swap preserves the vector state.
static void member_swap_self() {
    Vector<int> a{1, 2, 3};
    a.swap(a);

    CHK(a.size() == 3);
    CHK(a[0] == 1);
    CHK(a[1] == 2);
    CHK(a[2] == 3);
}

// Verifies the non-member swap exchanges vector contents.
static void free_swap_exchanges_contents() {
    Vector<int> a{1, 2, 3};
    Vector<int> b{4, 5};

    swap(a, b);

    CHK(a.size() == 2);
    CHK(a[0] == 4);
    CHK(b.size() == 3);
    CHK(b[0] == 1);
}

// Verifies the non-member swap behaves identically to member swap.
static void free_swap_matches_member_swap() {
    Vector<int> a{1, 2, 3};
    Vector<int> b{9, 8, 7, 6};

    swap(a, b);

    CHK(a.size() == 4);
    CHK(a[0] == 9);
    CHK(b.size() == 3);
    CHK(b[0] == 1);
}

// Verifies subscribed listeners remain attached to the swapped state.
static void swap_preserves_listeners() {
    ObservableVector<int> a{1, 2};
    ObservableVector<int> b{3, 4};

    int callCount = 0;
    (void)a.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++callCount; });

    a.swap(b);

    // Listener ownership moves with the swapped vector state.
    b.push_back(5);

    CHK(callCount == 1);
}

// Executes all swap test cases.
static void run_tests() {
    RUN(member_swap_exchanges_contents);
    RUN(member_swap_exchanges_capacity);
    RUN(member_swap_with_empty);
    RUN(member_swap_self);
    RUN(free_swap_exchanges_contents);
    RUN(free_swap_matches_member_swap);
    RUN(swap_preserves_listeners);
}

REGISTER_TEST_SUITE();