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

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies that a default-constructed vector starts empty with no capacity.
TEST(Construction, DefaultConstruction) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
    EXPECT_TRUE(v.empty());
}

// Verifies that count + value construction fills the vector with copies.
TEST(Construction, CountValueConstruction) {
    Vector<int> v(5, 7);
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v.capacity(), 5u);
    for (std::size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 7);
    }
}

// Verifies that count construction with no value uses the default T{}.
TEST(Construction, CountDefaultValueConstruction) {
    Vector<int> v(4);
    EXPECT_EQ(v.size(), 4u);
    for (std::size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

// Verifies that zero count construction produces an empty vector.
TEST(Construction, ZeroCountConstruction) {
    Vector<int> v(0, 9);
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
}

// Verifies that initializer list construction preserves order and size.
TEST(Construction, InitializerListConstruction) {
    Vector<int> v{1, 2, 3, 4};
    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

// Verifies that an empty initializer list produces an empty vector.
TEST(Construction, EmptyInitializerListConstruction) {
    Vector<int> v{};
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
}

// Verifies that iterator range construction copies elements via push_back.
TEST(Construction, IteratorRangeConstruction) {
    int src[] = {10, 20, 30};
    Vector<int> v(std::begin(src), std::end(src));
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

// Verifies that allocator construction produces a valid empty vector.
TEST(Construction, AllocatorConstruction) {
    std::allocator<int> alloc;
    Vector<int> v(alloc);
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
}

// Verifies that copy construction duplicates size and contents independently.
TEST(Construction, CopyConstruction) {
    Vector<int> original{1, 2, 3};
    Vector<int> copy(original);

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);

    copy[0] = 99;
    EXPECT_EQ(original[0], 1);
}

// Verifies that copy assignment reuses capacity when it already fits.
TEST(Construction, CopyAssignment) {
    Vector<int> source{1, 2, 3, 4, 5};
    Vector<int> target(10, 0);
    std::size_t capBefore = target.capacity();

    target = source;

    EXPECT_EQ(target.size(), source.size());
    EXPECT_EQ(target.capacity(), capBefore);
    for (std::size_t i = 0; i < target.size(); ++i) {
        EXPECT_EQ(target[i], source[i]);
    }
}

// Verifies that copy assignment grows capacity when the target is too small.
TEST(Construction, CopyAssignmentGrows) {
    Vector<int> source{1, 2, 3, 4, 5};
    Vector<int> target{1};

    target = source;

    EXPECT_EQ(target.size(), source.size());
    EXPECT_GE(target.capacity(), source.size());
}

// Verifies that self copy assignment leaves the vector unchanged.
TEST(Construction, SelfCopyAssignment) {
    Vector<int> v{1, 2, 3};
    v = v;

    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

// Verifies that move construction transfers ownership and empties the source.
TEST(Construction, MoveConstruction) {
    Vector<int> source{1, 2, 3};
    Vector<int> moved(std::move(source));

    EXPECT_EQ(moved.size(), 3u);
    EXPECT_EQ(moved[0], 1);

    // Intentional: verify moved-from state after move construction.
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(source.size(), 0u);
    EXPECT_EQ(source.capacity(), 0u);
}

// Verifies that move assignment transfers ownership and empties the source.
TEST(Construction, MoveAssignment) {
    Vector<int> source{1, 2, 3};
    Vector<int> target{9, 9};

    target = std::move(source);

    EXPECT_EQ(target.size(), 3u);
    EXPECT_EQ(target[0], 1);

    // Intentional: verify moved-from state after move construction.
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(source.size(), 0u);
    EXPECT_EQ(source.capacity(), 0u);
}

// Verifies that self move assignment leaves the vector unchanged.
TEST(Construction, SelfMoveAssignment) {
    Vector<int> v{1, 2, 3};
    v = std::move(v);

    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
}
