// Vector Growth Regression Suite
// Verifies capacity growth for non-default growth ratios, specifically the
// case where growCapacity()'s integer division (vcap_ * GrowthNum / GrowthDen)
// can return the *same* capacity as before when starting from a small
// capacity obtained outside of INITIAL_CAP (e.g. via the sized constructor
// or a small initializer list). When that happens, the next push_back
// writes to data_[vsize_] == data_[vcap_], one element past the allocated
// buffer.
//
// Covers:
// - Vector(count, value) with a small count and a fractional growth ratio
// - a small initializer_list with a fractional growth ratio
// - a larger starting capacity with the same ratio grows correctly
//   (isolates the bug to small starting capacities specifically)

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies that growing a vector constructed via Vector(count, value) with
// count = 1 and a 3/2 growth ratio actually increases capacity. 1 * 3 / 2
// truncates to 1 under integer division, so growCapacity() would return the
// vector's current capacity unchanged, and the following push_back would
// write past the end of the allocated buffer.
TEST(GrowthRegression, SmallCountConstructedCapacityGrowsOnPush) {
    Vector<int, std::allocator<int>, 3, 2> v(1, 5);
    std::size_t capBefore = v.capacity();
    EXPECT_EQ(capBefore, 1u);

    v.push_back(6);

    EXPECT_GT(v.capacity(), capBefore);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 6);
}

// Same scenario, but the small starting capacity comes from a one-element
// initializer_list instead of the sized constructor.
TEST(GrowthRegression, SmallInitializerListCapacityGrowsOnPush) {
    Vector<int, std::allocator<int>, 3, 2> v{7};
    std::size_t capBefore = v.capacity();
    EXPECT_EQ(capBefore, 1u);

    v.push_back(8);

    EXPECT_GT(v.capacity(), capBefore);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 7);
    EXPECT_EQ(v[1], 8);
}

// Contrast case: once the starting capacity is large enough that the
// fractional ratio doesn't truncate down to the same value, growth behaves
// correctly. This isolates the bug to small starting capacities rather than
// fractional ratios in general.
TEST(GrowthRegression, LargerStartingCapacityGrowsCorrectly) {
    Vector<int, std::allocator<int>, 3, 2> v;
    v.reserve(10);
    for (int i = 0; i < 10; ++i)
        v.push_back(i);

    std::size_t capBefore = v.capacity();
    EXPECT_EQ(capBefore, 10u);

    v.push_back(99); // 10 * 3 / 2 == 15, a genuine increase

    EXPECT_GT(v.capacity(), capBefore);
    EXPECT_EQ(v.size(), 11u);
    EXPECT_EQ(v[10], 99);
}

// Verifies repeated growth from a small, non-INITIAL_CAP starting capacity
// doesn't get permanently stuck: each push past capacity must strictly
// increase capacity, not just the first one.
TEST(GrowthRegression, RepeatedGrowthFromSmallCapacityNeverStalls) {
    Vector<int, std::allocator<int>, 3, 2> v(1, 0);

    for (int i = 1; i < 50; ++i) {
        std::size_t capBefore = v.capacity();
        if (v.size() == v.capacity()) {
            v.push_back(i);
            EXPECT_GT(v.capacity(), capBefore);
        } else {
            v.push_back(i);
        }
    }

    EXPECT_EQ(v.size(), 50u);
}
