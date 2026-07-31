// Vector move-only type example.
//
// Demonstrates:
// - Vector storing a move-only type (std::unique_ptr)
// - push_back and emplace_back with move-only elements
// - move-constructing a Vector of move-only elements
// - moving a single element out of the Vector

#include <support/framework.h>

#include <memory>

using namespace VectorPro;

static void run_examples() {
    // push_back and emplace_back with a move-only type.
    setTitle("Push Back Move-Only");

    Vector<std::unique_ptr<int>> v;
    v.push_back(std::make_unique<int>(1));
    v.push_back(std::make_unique<int>(2));
    v.emplace_back(std::make_unique<int>(3));

    std::cout << "Size     : " << v.size() << "\n";
    std::cout << "Elements : ";
    for (const auto& p : v)
        std::cout << *p << " ";
    std::cout << "\n\n";

    // Vector<T> itself becomes move-only when T is move-only:
    // copy construction and copy assignment are not available here.
    setTitle("Move Construction");

    Vector<std::unique_ptr<int>> moved(std::move(v));

    std::cout << "Moved-from size : " << v.size() << "\n";
    std::cout << "Moved-to size   : " << moved.size() << "\n\n";

    // Moving a single element out of the Vector leaves that slot empty
    // (a moved-from std::unique_ptr), while the Vector's size is unchanged.
    setTitle("Move Single Element");

    std::unique_ptr<int> extracted = std::move(moved[0]);

    std::cout << "Extracted value       : " << *extracted << "\n";
    std::cout << "Vector size unchanged : " << moved.size() << "\n";
    std::cout << "Slot now null         : " << (moved[0] == nullptr) << "\n";
}

REGISTER_EXAMPLE_SUITE();
