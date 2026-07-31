// Vector bounds checking example.
//
// Demonstrates:
// - at() throwing std::out_of_range on an invalid index
// - operator[] performing no bounds check (undefined behavior on misuse)
// - an off-by-one mistake using size() as an index
// - the correct pattern: bounds-check before use, or catch at()'s exception

#include <support/framework.h>

#include <stdexcept>

using namespace VectorPro;

static void run_examples() {
    // at() is bounds-checked and throws on an invalid index.
    setTitle("at() Throws");

    Vector<int> v{10, 20, 30};

    try {
        int value = v.at(5);
        (void)value;
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << "\n\n";
    }

    // operator[] performs no bounds check. Reading past the end is
    // undefined behavior — deliberately not executed in this example.
    setTitle("operator[] Has No Bounds Check");

    std::cout << "MISUSE: v[5] on a 3-element Vector compiles fine and may\n";
    std::cout << "        appear to \"work\", but it reads out-of-bounds\n";
    std::cout << "        memory. This is undefined behavior, not a bug\n";
    std::cout << "        Vector can detect for you.\n\n";

    // A common off-by-one mistake: using size() itself as the last index.
    setTitle("Off-By-One With size()");

    std::cout << "MISUSE: v[v.size()] looks tempting as \"the last element\"\n";
    std::cout << "        but valid indices only go up to size() - 1.\n";
    std::cout << "        Correct : v[v.size() - 1] or v.back()\n\n";

    std::cout << "Correct last element via back() : " << v.back() << "\n\n";

    // Correct pattern: bounds-check before use, or rely on at()'s exception.
    setTitle("Correct Pattern");

    std::size_t index = 5;

    if (index < v.size()) {
        std::cout << "In bounds : " << v[index] << "\n";
    } else {
        std::cout << "Index " << index << " is out of bounds (size = " << v.size() << ")\n";
    }
}

REGISTER_EXAMPLE_SUITE();
