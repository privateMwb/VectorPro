// Vector Ranges/STL Integrationl Test Suite
// Verifies VectorPro::Iterator's std::contiguous_iterator conformance by
// running standard algorithms (both classic <algorithm> and <ranges>-based)
// directly over a Vector, end to end.
//
// Covers:
// - Iterator satisfies std::contiguous_iterator at compile time
// - std::sort / std::ranges::sort
// - std::find / std::ranges::find
// - std::reverse
// - std::accumulate
// - std::min_element / std::max_element
// - std::ranges::for_each
// - std::ranges::distance matches size()
// - std::back_inserter works with Vector::push_back

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

#include <algorithm>
#include <iterator>
#include <numeric>
#include <ranges>

using namespace VectorPro;

// Compile-time check: VectorPro::Iterator must satisfy std::contiguous_iterator
// for both the mutable and const iterator forms.
static_assert(std::contiguous_iterator<Vector<int>::iterator>);
static_assert(std::contiguous_iterator<Vector<int>::const_iterator>);

// Verifies std::sort works directly over Vector's iterator range.
TEST(RangesStl, StdSortOverVector) {
    Vector<int> v{5, 3, 1, 4, 2};
    std::sort(v.begin(), v.end());

    EXPECT_EQ(v.size(), 5u);
    for (std::size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i) + 1);
    }
}

// Verifies std::ranges::sort works directly on a Vector (as a range).
TEST(RangesStl, RangesSortOverVector) {
    Vector<int> v{9, 1, 8, 2, 7};
    std::ranges::sort(v);

    Vector<int> expected{1, 2, 7, 8, 9};
    EXPECT_EQ(v, expected);
}

// Verifies std::find locates the correct element and returns an iterator
// usable with the vector's own iterator arithmetic.
TEST(RangesStl, StdFindOverVector) {
    Vector<int> v{10, 20, 30, 40};
    auto it = std::find(v.begin(), v.end(), 30);

    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 30);
    EXPECT_EQ(it - v.begin(), 2);
}

// Verifies std::ranges::find works directly on a Vector.
TEST(RangesStl, RangesFindOverVector) {
    Vector<int> v{10, 20, 30, 40};
    auto it = std::ranges::find(v, 40);

    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 40);
}

// Verifies std::reverse works directly over Vector's iterator range.
TEST(RangesStl, StdReverseOverVector) {
    Vector<int> v{1, 2, 3, 4, 5};
    std::reverse(v.begin(), v.end());

    Vector<int> expected{5, 4, 3, 2, 1};
    EXPECT_EQ(v, expected);
}

// Verifies std::accumulate sums a Vector's elements correctly.
TEST(RangesStl, StdAccumulateOverVector) {
    Vector<int> v{1, 2, 3, 4, 5};
    int sum = std::accumulate(v.begin(), v.end(), 0);

    EXPECT_EQ(sum, 15);
}

// Verifies std::min_element / std::max_element over a Vector.
TEST(RangesStl, StdMinMaxElementOverVector) {
    Vector<int> v{4, 1, 7, 2, 9, 3};

    auto minIt = std::min_element(v.begin(), v.end());
    auto maxIt = std::max_element(v.begin(), v.end());

    EXPECT_EQ(*minIt, 1);
    EXPECT_EQ(*maxIt, 9);
}

// Verifies std::ranges::for_each visits every element in order exactly once.
TEST(RangesStl, RangesForEachOverVector) {
    Vector<int> v{1, 2, 3, 4};
    int sum = 0;
    std::size_t visits = 0;

    std::ranges::for_each(v, [&](int x) {
        sum += x;
        ++visits;
    });

    EXPECT_EQ(sum, 10);
    EXPECT_EQ(visits, 4u);
}

// Verifies std::ranges::distance matches Vector::size().
TEST(RangesStl, RangesDistanceMatchesSize) {
    Vector<int> v{1, 2, 3, 4, 5, 6};

    EXPECT_EQ(static_cast<std::size_t>(std::ranges::distance(v.begin(), v.end())), v.size());
    EXPECT_EQ(static_cast<std::size_t>(std::ranges::distance(v)), v.size());
}

// Verifies std::back_inserter works with Vector via its push_back overloads.
TEST(RangesStl, BackInserterPopulatesVector) {
    Vector<int> v;
    Vector<int> source{1, 2, 3, 4, 5};

    std::copy(source.begin(), source.end(), std::back_inserter(v));

    EXPECT_EQ(v, source);
}
