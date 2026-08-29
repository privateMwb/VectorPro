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

#include <support/framework.h>

using namespace VectorPro;

// Verifies that growing a vector constructed via Vector(count, value) with
// count = 1 and a 3/2 growth ratio actually increases capacity. 1 * 3 / 2
// truncates to 1 under integer division, so growCapacity() would return the
// vector's current capacity unchanged, and the following push_back would
// write past the end of the allocated buffer.
static void small_count_constructed_capacity_grows_on_push() {
    Vector<int, std::allocator<int>, 3, 2> v(1, 5);
    std::size_t capBefore = v.capacity();
    CHK(capBefore == 1);

    v.push_back(6);

    CHK(v.capacity() > capBefore);
    CHK(v.size() == 2);
    CHK(v[0] == 5);
    CHK(v[1] == 6);
}

// Same scenario, but the small starting capacity comes from a one-element
// initializer_list instead of the sized constructor.
static void small_initializer_list_capacity_grows_on_push() {
    Vector<int, std::allocator<int>, 3, 2> v{7};
    std::size_t capBefore = v.capacity();
    CHK(capBefore == 1);

    v.push_back(8);

    CHK(v.capacity() > capBefore);
    CHK(v.size() == 2);
    CHK(v[0] == 7);
    CHK(v[1] == 8);
}

// Contrast case: once the starting capacity is large enough that the
// fractional ratio doesn't truncate down to the same value, growth behaves
// correctly. This isolates the bug to small starting capacities rather than
// fractional ratios in general.
static void larger_starting_capacity_grows_correctly() {
    Vector<int, std::allocator<int>, 3, 2> v;
    v.reserve(10);
    for (int i = 0; i < 10; ++i)
        v.push_back(i);

    std::size_t capBefore = v.capacity();
    CHK(capBefore == 10);

    v.push_back(99); // 10 * 3 / 2 == 15, a genuine increase

    CHK(v.capacity() > capBefore);
    CHK(v.size() == 11);
    CHK(v[10] == 99);
}

// Verifies repeated growth from a small, non-INITIAL_CAP starting capacity
// doesn't get permanently stuck: each push past capacity must strictly
// increase capacity, not just the first one.
static void repeated_growth_from_small_capacity_never_stalls() {
    Vector<int, std::allocator<int>, 3, 2> v(1, 0);

    for (int i = 1; i < 50; ++i) {
        std::size_t capBefore = v.capacity();
        if (v.size() == v.capacity()) {
            v.push_back(i);
            CHK(v.capacity() > capBefore);
        } else {
            v.push_back(i);
        }
    }

    CHK(v.size() == 50);
}

// Executes all growth regression test cases.
static void run_tests() {
    RUN(small_count_constructed_capacity_grows_on_push);
    RUN(small_initializer_list_capacity_grows_on_push);
    RUN(larger_starting_capacity_grows_correctly);
    RUN(repeated_growth_from_small_capacity_never_stalls);
}

REGISTER_TEST_SUITE();
