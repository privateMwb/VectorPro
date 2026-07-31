// Vector modifiers example.
//
// Demonstrates:
// - insert at the front, middle, and end
// - erase of a single element and a range
// - emplace
// - remove_if
// - pop_back
// - clear

#include <support/framework.h>

using namespace VectorPro;

// Prints the contents of a vector.
static void printVector(const Vector<int>& v) {
    for (int x : v)
        std::cout << x << ' ';
    std::cout << "\n";
}

static void run_examples() {
    // Insert elements at different positions.
    setTitle("Insert");

    Vector<int> v{2, 3, 4};

    std::cout << "Before insert        : ";
    printVector(v);

    (void)v.insert(v.cbegin(), 1);
    std::cout << "After insert front   : ";
    printVector(v);

    (void)v.insert(v.cbegin() + 2, 99);
    std::cout << "After insert middle  : ";
    printVector(v);

    (void)v.insert(v.cend(), 100);
    std::cout << "After insert end     : ";
    printVector(v);
    std::cout << "\n";

    // Erase a single element, then erase a range.
    setTitle("Erase");

    std::cout << "Before erase         : ";
    printVector(v);

    (void)v.erase(v.cbegin() + 2);
    std::cout << "After erase single   : ";
    printVector(v);

    (void)v.erase(v.cbegin() + 1, v.cbegin() + 3);
    std::cout << "After erase range    : ";
    printVector(v);
    std::cout << "\n";

    // Construct elements directly in place.
    setTitle("Emplace");

    Vector<int> e{1, 2, 5};

    std::cout << "Before emplace       : ";
    printVector(e);

    (void)e.emplace(e.cbegin() + 2, 3);
    (void)e.emplace(e.cend(), 6);

    std::cout << "After emplace        : ";
    printVector(e);
    std::cout << "\n";

    // Remove all even values.
    setTitle("Remove If");

    Vector<int> r{1, 2, 3, 4, 5, 6, 7, 8};

    std::cout << "Before remove_if     : ";
    printVector(r);

    std::size_t removed = r.remove_if([](const int& x) { return x % 2 == 0; });

    std::cout << "After remove_if      : ";
    printVector(r);
    std::cout << "Removed count        : " << removed << "\n\n";

    // Remove the last element, then clear the container.
    setTitle("Pop Back And Clear");

    std::cout << "Before pop_back      : ";
    printVector(r);

    r.pop_back();

    std::cout << "After pop_back       : ";
    printVector(r);

    r.clear();

    std::cout << "After clear          : size = " << r.size() << ", empty = " << r.empty() << "\n";
}

REGISTER_EXAMPLE_SUITE();