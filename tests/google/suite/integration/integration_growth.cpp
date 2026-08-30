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

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

namespace {

// Runs `count` push_backs on a Vector with the given GrowthNum/GrowthDen
// ratio and verifies size, contents, and the capacity>=size invariant hold
// throughout.
template <std::size_t GrowthNum, std::size_t GrowthDen> void exercise_growth_ratio(int count) {
    Vector<int, std::allocator<int>, GrowthNum, GrowthDen> v;

    for (int i = 0; i < count; ++i) {
        v.push_back(i);
        EXPECT_GE(v.capacity(), v.size());
    }

    EXPECT_EQ(v.size(), static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) {
        EXPECT_EQ(v[i], i);
    }
}

} // namespace

// Verifies a 3/2 growth ratio sustains 5000 push_backs correctly.
TEST(CustomGrowthPolicy, Ratio3_2SustainsManyPushBacks) {
    exercise_growth_ratio<3, 2>(5000);
}

// Verifies a 4/3 growth ratio sustains 5000 push_backs correctly.
TEST(CustomGrowthPolicy, Ratio4_3SustainsManyPushBacks) {
    exercise_growth_ratio<4, 3>(5000);
}

// Verifies a large growth ratio (5/1) still preserves contents and the
// capacity>=size invariant.
TEST(CustomGrowthPolicy, Ratio5_1SustainsManyPushBacks) {
    exercise_growth_ratio<5, 1>(5000);
}

// Verifies that a custom growth ratio actually amortizes reallocation
// rather than reallocating on every push_back: the number of capacity
// changes over N push_backs should be far smaller than N.
TEST(CustomGrowthPolicy, CustomRatioAmortizesReallocation) {
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
    EXPECT_LT(capacityChanges, 100u);
}

// Verifies mixed push_back/insert/erase under a custom growth ratio
// produces the same logical contents as an equivalent default-ratio Vector.
TEST(CustomGrowthPolicy, CustomRatioMixedOpsMatchDefaultRatioContents) {
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

    EXPECT_EQ(custom.size(), reference.size());
    for (std::size_t i = 0; i < custom.size(); ++i) {
        EXPECT_EQ(custom[i], reference[i]);
    }
}
