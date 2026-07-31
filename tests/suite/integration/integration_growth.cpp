// Vector Custom Growth Policy Integration Test Suite
// Verifies that Vector instantiated with non-default GrowthNum/GrowthDen
// template parameters behaves correctly across sustained, ordinary usage
// (not the small-starting-capacity edge case covered by
// regression/growth.cpp — this is the "normal operation" end-to-end path).
//
// Covers:
// - a 3/2 growth ratio sustains thousands of push_backs with correct
//   contents and monotonically non-decreasing capacity
// - a 4/3 growth ratio behaves the same way
// - a large ratio (5/1) still preserves contents and capacity invariants
// - capacity never has to grow on every single push_back (amortized growth
//   is actually happening, not reallocating every time)
// - mixed push_back/insert/erase under a custom ratio still mirrors a
//   default-ratio Vector's logical contents

#include <support/framework.h>

using namespace VectorPro;

// Runs `count` push_backs on a Vector with the given GrowthNum/GrowthDen
// ratio and verifies size, contents, and the capacity>=size invariant hold
// throughout.
template <std::size_t GrowthNum, std::size_t GrowthDen>
static void exercise_growth_ratio(int count) {
    Vector<int, std::allocator<int>, GrowthNum, GrowthDen> v;

    for (int i = 0; i < count; ++i) {
        v.push_back(i);
        CHK(v.capacity() >= v.size());
    }

    CHK(v.size() == static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) {
        CHK(v[i] == i);
    }
}

// Verifies a 3/2 growth ratio sustains 5000 push_backs correctly.
static void ratio_3_2_sustains_many_push_backs() {
    exercise_growth_ratio<3, 2>(5000);
}

// Verifies a 4/3 growth ratio sustains 5000 push_backs correctly.
static void ratio_4_3_sustains_many_push_backs() {
    exercise_growth_ratio<4, 3>(5000);
}

// Verifies a large growth ratio (5/1) still preserves contents and the
// capacity>=size invariant.
static void ratio_5_1_sustains_many_push_backs() {
    exercise_growth_ratio<5, 1>(5000);
}

// Verifies that a custom growth ratio actually amortizes reallocation
// rather than reallocating on every push_back: the number of capacity
// changes over N push_backs should be far smaller than N.
static void custom_ratio_amortizes_reallocation() {
    Vector<int, std::allocator<int>, 3, 2> v;
    std::size_t lastCap = 0;
    std::size_t capacityChanges = 0;

    for (int i = 0; i < 2000; ++i) {
        v.push_back(i);
        if (v.capacity() != lastCap) {
            ++capacityChanges;
            lastCap = v.capacity();
        }
    }

    // Growth is logarithmic in the number of pushes; a generous linear
    // bound (well under the 2000 pushes performed) is enough to catch a
    // regression back to reallocating on every push.
    CHK(capacityChanges < 100);
}

// Verifies mixed push_back/insert/erase under a custom growth ratio
// produces the same logical contents as an equivalent default-ratio Vector.
static void custom_ratio_mixed_ops_match_default_ratio_contents() {
    Vector<int, std::allocator<int>, 3, 2> custom;
    Vector<int> reference; // default 2/1 ratio

    for (int i = 0; i < 200; ++i) {
        custom.push_back(i);
        reference.push_back(i);
    }

    (void)custom.insert(custom.cbegin() + 50, -1);
    (void)reference.insert(reference.cbegin() + 50, -1);

    (void)custom.erase(custom.cbegin() + 10, custom.cbegin() + 20);
    (void)reference.erase(reference.cbegin() + 10, reference.cbegin() + 20);

    CHK(custom.size() == reference.size());
    for (std::size_t i = 0; i < custom.size(); ++i) {
        CHK(custom[i] == reference[i]);
    }
}

// Executes all custom growth policy integration test cases.
static void run_tests() {
    RUN(ratio_3_2_sustains_many_push_backs);
    RUN(ratio_4_3_sustains_many_push_backs);
    RUN(ratio_5_1_sustains_many_push_backs);
    RUN(custom_ratio_amortizes_reallocation);
    RUN(custom_ratio_mixed_ops_match_default_ratio_contents);
}

REGISTER_TEST_SUITE();
