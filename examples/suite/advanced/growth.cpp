// Vector growth policy example.
//
// Demonstrates:
// - Default growth factor (2/1)
// - Custom growth factors (3/2, 4/1)
// - Reallocation behavior differences across policies

#include <support/framework.h>

using namespace VectorPro;

// Counts how many times capacity changes during pushes.
// Used as a proxy for allocation/reallocation frequency.
template <typename VecType> static std::size_t countReallocations(VecType& v, int count) {
    std::size_t reallocations = 0;
    std::size_t lastCap = v.capacity();

    for (int i = 0; i < count; ++i) {
        v.push_back(i);

        if (v.capacity() != lastCap) {
            ++reallocations;
            lastCap = v.capacity();
        }
    }

    return reallocations;
}

static void run_examples() {
    constexpr int kPushCount = 10000;

    // Default growth policy (2/1): capacity typically doubles per reallocation.
    setTitle("Default Growth (2/1)");

    Vector<int> defaultGrowth;
    std::size_t defaultReallocs = countReallocations(defaultGrowth, kPushCount);

    std::cout << "Final size         : " << defaultGrowth.size() << "\n";
    std::cout << "Final capacity     : " << defaultGrowth.capacity() << "\n";
    std::cout << "Reallocation count : " << defaultReallocs << "\n\n";

    // Growth policy (3/2): smaller growth steps, more frequent reallocations.
    setTitle("Conservative Growth (3/2)");

    Vector<int, std::allocator<int>, 3, 2> conservativeGrowth;
    std::size_t conservativeReallocs = countReallocations(conservativeGrowth, kPushCount);

    std::cout << "Final size         : " << conservativeGrowth.size() << "\n";
    std::cout << "Final capacity     : " << conservativeGrowth.capacity() << "\n";
    std::cout << "Reallocation count : " << conservativeReallocs << "\n\n";

    // Growth policy (4/1): larger growth steps, fewer reallocations.
    setTitle("Aggressive Growth (4/1)");

    Vector<int, std::allocator<int>, 4, 1> aggressiveGrowth;
    std::size_t aggressiveReallocs = countReallocations(aggressiveGrowth, kPushCount);

    std::cout << "Final size         : " << aggressiveGrowth.size() << "\n";
    std::cout << "Final capacity     : " << aggressiveGrowth.capacity() << "\n";
    std::cout << "Reallocation count : " << aggressiveReallocs << "\n\n";

    // Side-by-side comparison of policies.
    setTitle("Comparison");

    std::cout << "Policy          | Reallocations | Final Capacity | Wasted Slots\n";
    std::cout << "----------------|---------------|----------------|-------------\n";
    std::cout << "2/1 (default)   | " << defaultReallocs << "\t\t| " << defaultGrowth.capacity()
              << "\t\t | " << (defaultGrowth.capacity() - defaultGrowth.size()) << "\n";

    std::cout << "3/2 (careful)   | " << conservativeReallocs << "\t\t| "
              << conservativeGrowth.capacity() << "\t\t | "
              << (conservativeGrowth.capacity() - conservativeGrowth.size()) << "\n";

    std::cout << "4/1 (aggressive)| " << aggressiveReallocs << "\t\t| "
              << aggressiveGrowth.capacity() << "\t\t | "
              << (aggressiveGrowth.capacity() - aggressiveGrowth.size()) << "\n";
}

REGISTER_EXAMPLE_SUITE();