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

#include <algorithm>
#include <iterator>
#include <numeric>
#include <ranges>
#include <support/framework.h>

using namespace VectorPro;

// Compile-time check: VectorPro::Iterator must satisfy std::contiguous_iterator
// for both the mutable and const iterator forms.
static_assert(std::contiguous_iterator<Vector<int>::iterator>);
static_assert(std::contiguous_iterator<Vector<int>::const_iterator>);

// Verifies std::sort works directly over Vector's iterator range.
static void std_sort_over_vector() {
    Vector<int> v{5, 3, 1, 4, 2};
    std::sort(v.begin(), v.end());

    CHK(v.size() == 5);
    for (std::size_t i = 0; i < v.size(); ++i) {
        CHK(v[i] == static_cast<int>(i) + 1);
    }
}

// Verifies std::ranges::sort works directly on a Vector (as a range).
static void ranges_sort_over_vector() {
    Vector<int> v{9, 1, 8, 2, 7};
    std::ranges::sort(v);

    Vector<int> expected{1, 2, 7, 8, 9};
    CHK(v == expected);
}

// Verifies std::find locates the correct element and returns an iterator
// usable with the vector's own iterator arithmetic.
static void std_find_over_vector() {
    Vector<int> v{10, 20, 30, 40};
    auto it = std::find(v.begin(), v.end(), 30);

    CHK(it != v.end());
    CHK(*it == 30);
    CHK(it - v.begin() == 2);
}

// Verifies std::ranges::find works directly on a Vector.
static void ranges_find_over_vector() {
    Vector<int> v{10, 20, 30, 40};
    auto it = std::ranges::find(v, 40);

    CHK(it != v.end());
    CHK(*it == 40);
}

// Verifies std::reverse works directly over Vector's iterator range.
static void std_reverse_over_vector() {
    Vector<int> v{1, 2, 3, 4, 5};
    std::reverse(v.begin(), v.end());

    Vector<int> expected{5, 4, 3, 2, 1};
    CHK(v == expected);
}

// Verifies std::accumulate sums a Vector's elements correctly.
static void std_accumulate_over_vector() {
    Vector<int> v{1, 2, 3, 4, 5};
    int sum = std::accumulate(v.begin(), v.end(), 0);

    CHK(sum == 15);
}

// Verifies std::min_element / std::max_element over a Vector.
static void std_min_max_element_over_vector() {
    Vector<int> v{4, 1, 7, 2, 9, 3};

    auto minIt = std::min_element(v.begin(), v.end());
    auto maxIt = std::max_element(v.begin(), v.end());

    CHK(*minIt == 1);
    CHK(*maxIt == 9);
}

// Verifies std::ranges::for_each visits every element in order exactly once.
static void ranges_for_each_over_vector() {
    Vector<int> v{1, 2, 3, 4};
    int sum = 0;
    std::size_t visits = 0;

    std::ranges::for_each(v, [&](int x) {
        sum += x;
        ++visits;
    });

    CHK(sum == 10);
    CHK(visits == 4);
}

// Verifies std::ranges::distance matches Vector::size().
static void ranges_distance_matches_size() {
    Vector<int> v{1, 2, 3, 4, 5, 6};

    CHK(static_cast<std::size_t>(std::ranges::distance(v.begin(), v.end())) == v.size());
    CHK(static_cast<std::size_t>(std::ranges::distance(v)) == v.size());
}

// Verifies std::back_inserter works with Vector via its push_back overloads.
static void back_inserter_populates_vector() {
    Vector<int> v;
    Vector<int> source{1, 2, 3, 4, 5};

    std::copy(source.begin(), source.end(), std::back_inserter(v));

    CHK(v == source);
}

// Executes all ranges/STL integration test cases.
static void run_tests() {
    RUN(std_sort_over_vector);
    RUN(ranges_sort_over_vector);
    RUN(std_find_over_vector);
    RUN(ranges_find_over_vector);
    RUN(std_reverse_over_vector);
    RUN(std_accumulate_over_vector);
    RUN(std_min_max_element_over_vector);
    RUN(ranges_for_each_over_vector);
    RUN(ranges_distance_matches_size);
    RUN(back_inserter_populates_vector);
}

REGISTER_TEST_SUITE();
