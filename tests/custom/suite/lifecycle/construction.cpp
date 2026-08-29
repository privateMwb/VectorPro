// Vector constructor test suite.
//
// Coverage:
// - Default construction
// - Count + value construction
// - Count + default value construction
// - Initializer list construction
// - Iterator range construction
// - Allocator construction
// - Copy construction
// - Copy assignment
// - Move construction
// - Move assignment
// - Self copy assignment
// - Self move assignment

#include <support/framework.h>

using namespace VectorPro;

// Verifies that a default-constructed vector starts empty with no capacity.
static void default_construction() {
    Vector<int> v;
    CHK(v.size() == 0);
    CHK(v.capacity() == 0);
    CHK(v.empty() == true);
}

// Verifies that count + value construction fills the vector with copies.
static void count_value_construction() {
    Vector<int> v(5, 7);
    CHK(v.size() == 5);
    CHK(v.capacity() == 5);
    for (std::size_t i = 0; i < v.size(); ++i) {
        CHK(v[i] == 7);
    }
}

// Verifies that count construction with no value uses the default T{}.
static void count_default_value_construction() {
    Vector<int> v(4);
    CHK(v.size() == 4);
    for (std::size_t i = 0; i < v.size(); ++i) {
        CHK(v[i] == 0);
    }
}

// Verifies that zero count construction produces an empty vector.
static void zero_count_construction() {
    Vector<int> v(0, 9);
    CHK(v.size() == 0);
    CHK(v.capacity() == 0);
}

// Verifies that initializer list construction preserves order and size.
static void initializer_list_construction() {
    Vector<int> v{1, 2, 3, 4};
    CHK(v.size() == 4);
    CHK(v[0] == 1);
    CHK(v[1] == 2);
    CHK(v[2] == 3);
    CHK(v[3] == 4);
}

// Verifies that an empty initializer list produces an empty vector.
static void empty_initializer_list_construction() {
    Vector<int> v{};
    CHK(v.size() == 0);
    CHK(v.capacity() == 0);
}

// Verifies that iterator range construction copies elements via push_back.
static void iterator_range_construction() {
    int src[] = {10, 20, 30};
    Vector<int> v(std::begin(src), std::end(src));
    CHK(v.size() == 3);
    CHK(v[0] == 10);
    CHK(v[1] == 20);
    CHK(v[2] == 30);
}

// Verifies that allocator construction produces a valid empty vector.
static void allocator_construction() {
    std::allocator<int> alloc;
    Vector<int> v(alloc);
    CHK(v.size() == 0);
    CHK(v.capacity() == 0);
}

// Verifies that copy construction duplicates size and contents independently.
static void copy_construction() {
    Vector<int> original{1, 2, 3};
    Vector<int> copy(original);

    CHK(copy.size() == original.size());
    CHK(copy[0] == 1);
    CHK(copy[1] == 2);
    CHK(copy[2] == 3);

    copy[0] = 99;
    CHK(original[0] == 1);
}

// Verifies that copy assignment reuses capacity when it already fits.
static void copy_assignment() {
    Vector<int> source{1, 2, 3, 4, 5};
    Vector<int> target(10, 0);
    std::size_t capBefore = target.capacity();

    target = source;

    CHK(target.size() == source.size());
    CHK(target.capacity() == capBefore);
    for (std::size_t i = 0; i < target.size(); ++i) {
        CHK(target[i] == source[i]);
    }
}

// Verifies that copy assignment grows capacity when the target is too small.
static void copy_assignment_grows() {
    Vector<int> source{1, 2, 3, 4, 5};
    Vector<int> target{1};

    target = source;

    CHK(target.size() == source.size());
    CHK(target.capacity() >= source.size());
}

// Verifies that self copy assignment leaves the vector unchanged.
static void self_copy_assignment() {
    Vector<int> v{1, 2, 3};
    v = v;

    CHK(v.size() == 3);
    CHK(v[0] == 1);
    CHK(v[1] == 2);
    CHK(v[2] == 3);
}

// Verifies that move construction transfers ownership and empties the source.
static void move_construction() {
    Vector<int> source{1, 2, 3};
    Vector<int> moved(std::move(source));

    CHK(moved.size() == 3);
    CHK(moved[0] == 1);

    // Intentional: verify moved-from state after move construction.
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    CHK(source.size() == 0);
    CHK(source.capacity() == 0);
}

// Verifies that move assignment transfers ownership and empties the source.
static void move_assignment() {
    Vector<int> source{1, 2, 3};
    Vector<int> target{9, 9};

    target = std::move(source);

    CHK(target.size() == 3);
    CHK(target[0] == 1);

    // Intentional: verify moved-from state after move construction.
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    CHK(source.size() == 0);
    CHK(source.capacity() == 0);
}

// Verifies that self move assignment leaves the vector unchanged.
static void self_move_assignment() {
    Vector<int> v{1, 2, 3};
    v = std::move(v);

    CHK(v.size() == 3);
    CHK(v[0] == 1);
}

// Executes all constructor test cases.
static void run_tests() {
    RUN(default_construction);
    RUN(count_value_construction);
    RUN(count_default_value_construction);
    RUN(zero_count_construction);
    RUN(initializer_list_construction);
    RUN(empty_initializer_list_construction);
    RUN(iterator_range_construction);
    RUN(allocator_construction);
    RUN(copy_construction);
    RUN(copy_assignment);
    RUN(copy_assignment_grows);
    RUN(self_copy_assignment);
    RUN(move_construction);
    RUN(move_assignment);
    RUN(self_move_assignment);
}

REGISTER_TEST_SUITE();