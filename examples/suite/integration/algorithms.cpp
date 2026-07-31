// Vector algorithms example.
//
// Demonstrates:
// - Compatibility with standard algorithms (<algorithm>, <numeric>)
// - Usage with std::ranges algorithms
// - std::span view over Vector storage
// - Sorting, transforming, and aggregating Vector data

#include <support/framework.h>

#include <algorithm>
#include <numeric>
#include <ranges>

using namespace VectorPro;

// Prints a Vector's contents on one line.
static void printVector(const Vector<int>& v) {
    for (int x : v)
        std::cout << x << " ";
    std::cout << "\n";
}

static void run_examples() {
    // std::sort with contiguous iterators.
    setTitle("std::sort");

    Vector<int> v{5, 3, 1, 4, 2};
    std::cout << "Before sort : ";
    printVector(v);

    std::sort(v.begin(), v.end());
    std::cout << "After sort  : ";
    printVector(v);
    std::cout << "\n";

    // std::transform.
    setTitle("std::transform");

    Vector<int> doubled(v.size(), 0);
    std::transform(v.begin(), v.end(), doubled.begin(), [](int x) { return x * 2; });

    std::cout << "Doubled : ";
    printVector(doubled);
    std::cout << "\n";

    // std::accumulate.
    setTitle("std::accumulate");

    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Sum : " << sum << "\n\n";

    // std::ranges algorithms over Vector.
    setTitle("std::ranges");

    Vector<int> evens;
    std::ranges::copy_if(v, std::back_inserter(evens), [](int x) { return x % 2 == 0; });

    auto maxIt = std::ranges::max_element(v);
    std::cout << "Max element : " << *maxIt << "\n";

    bool anyOverThree = std::ranges::any_of(v, [](int x) { return x > 3; });
    std::cout << "Any > 3     : " << anyOverThree << "\n\n";

    // std::span view over Vector storage.
    setTitle("std::span Interop");

    std::span<int> sp = v.as_span();
    std::cout << "Span size     : " << sp.size() << "\n";
    std::cout << "Span first    : " << sp.front() << "\n";
    std::cout << "Span last     : " << sp.back() << "\n";

    auto sub = sp.subspan(1, 3);
    std::cout << "Subspan(1,3)  : ";
    for (int x : sub)
        std::cout << x << " ";
    std::cout << "\n";
}

REGISTER_EXAMPLE_SUITE();