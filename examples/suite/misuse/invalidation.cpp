// Vector iterator invalidation example.
//
// Demonstrates:
// - iterators/pointers invalidated by a growth-triggering push_back
// - iterators/pointers invalidated by insert() and erase()
// - the correct pattern: re-fetch iterators after a mutating call

#include <support/framework.h>

using namespace VectorPro;

static void run_examples() {
    // A push_back that forces reallocation invalidates every existing
    // iterator, pointer, and reference into the Vector.
    setTitle("Growth Invalidates Iterators");

    Vector<int> v;
    v.reserve(4);
    v.push_back(1);
    v.push_back(2);

    int* stalePtr = &v[0];
    auto staleIt = v.begin();

    std::cout << "Capacity before : " << v.capacity() << "\n";

    for (int i = 0; i < 10; ++i)
        v.push_back(i); // eventually exceeds capacity(4) and reallocates

    std::cout << "Capacity after  : " << v.capacity() << "\n";
    std::cout << "MISUSE: stalePtr / staleIt now point into freed memory.\n";
    std::cout << "        Dereferencing them here is undefined behavior —\n";
    std::cout << "        deliberately not done in this example.\n\n";
    (void)stalePtr;
    (void)staleIt;

    // insert() shifts every element from the insertion point onward,
    // invalidating iterators/pointers at or after that point.
    setTitle("Insert Invalidates Iterators");

    Vector<int> w{1, 2, 3, 4, 5};
    auto itToThird = w.cbegin() + 2;

    (void)w.insert(w.cbegin(), 0);

    std::cout << "MISUSE: itToThird was taken before insert() and no longer\n";
    std::cout << "        refers to the same logical element.\n\n";
    (void)itToThird;

    // erase() invalidates the erased element's iterator and every
    // iterator/pointer after it.
    setTitle("Erase Invalidates Iterators");

    Vector<int> x{1, 2, 3, 4, 5};
    auto itPastErase = x.cbegin() + 3;

    (void)x.erase(x.cbegin() + 1);

    std::cout << "MISUSE: itPastErase was taken before erase() and now refers\n";
    std::cout << "        to a shifted (or out-of-range) position.\n\n";
    (void)itPastErase;

    // Correct pattern: re-fetch iterators/pointers after any call that
    // may reallocate or shift elements.
    setTitle("Correct Pattern");

    Vector<int> y{1, 2, 3};
    (void)y.insert(y.cbegin(), 0);

    auto freshIt = y.cbegin() + 2; // re-fetched after the mutating call
    std::cout << "Safe access after re-fetch : " << *freshIt << "\n";
}

REGISTER_EXAMPLE_SUITE();
