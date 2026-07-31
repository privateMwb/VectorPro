// Basic Vector example.
//
// Demonstrates:
// - construction from an initializer list
// - push_back growth
// - element access (front, back, operator[], at)
// - range-based iteration
// - reserve and shrink_to_fit

#include <support/framework.h>

using namespace VectorPro;

static void run_examples() {
    // Construction from an initializer list.
    setTitle("Construction");

    Vector<int> v{1, 2, 3};

    std::cout << "Size     : " << v.size() << "\n";
    std::cout << "Capacity : " << v.capacity() << "\n";
    std::cout << "Empty    : " << v.empty() << "\n\n";

    // Append elements and observe capacity growth.
    setTitle("Push Back");

    for (int i = 4; i <= 8; ++i)
        v.push_back(i);

    std::cout << "Size after push_back     : " << v.size() << "\n";
    std::cout << "Capacity after push_back : " << v.capacity() << "\n\n";

    // Access elements using the available accessors.
    setTitle("Element Access");

    std::cout << "front() : " << v.front() << "\n";
    std::cout << "back()  : " << v.back() << "\n";
    std::cout << "v[3]    : " << v[3] << "\n";
    std::cout << "v.at(3) : " << v.at(3) << "\n\n";

    // Iterate over every element.
    setTitle("Iteration");

    std::cout << "Elements : ";
    for (int x : v)
        std::cout << x << ' ';
    std::cout << "\n\n";

    // Reserve additional capacity, then release unused storage.
    setTitle("Capacity");

    v.reserve(50);
    std::cout << "Capacity after reserve(50)     : " << v.capacity() << "\n";

    v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() : " << v.capacity() << "\n";
}

REGISTER_EXAMPLE_SUITE();