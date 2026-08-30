// Vector Access Test Suite
// Verifies element accessors and view operations.
//
// Covers:
// - as_span
// - data_ptr
// - at
// - front and back
// - operator[]
// - const accessors

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies as_span() exposes the vector contents as a contiguous view.
TEST(Access, AsSpanBasic) {
    Vector<int> v{1, 2, 3};
    auto span = v.as_span();

    EXPECT_EQ(span.size(), 3u);
    EXPECT_EQ(span[0], 1);
    EXPECT_EQ(span[2], 3);
}

// Verifies modifications through as_span() are reflected in the vector.
TEST(Access, AsSpanReflectsMutation) {
    Vector<int> v{1, 2, 3};
    auto span = v.as_span();
    span[0] = 99;

    EXPECT_EQ(v[0], 99);
}

// Verifies the const overload of as_span() provides read-only access.
TEST(Access, AsSpanConst) {
    const Vector<int> v{1, 2, 3};
    auto span = v.as_span();

    EXPECT_EQ(span.size(), 3u);
    EXPECT_EQ(span[1], 2);
}

// Verifies data_ptr() returns a pointer to the underlying storage.
TEST(Access, DataPtrBasic) {
    Vector<int> v{1, 2, 3};
    int* p = v.data_ptr();

    EXPECT_EQ(p[0], 1);
    EXPECT_EQ(p, &v[0]);
}

// Verifies data_ptr() returns nullptr for an empty vector.
TEST(Access, DataPtrNullWhenEmpty) {
    Vector<int> v;
    EXPECT_EQ(v.data_ptr(), nullptr);
}

// Verifies at() returns the requested element for a valid index.
TEST(Access, AtValidIndex) {
    Vector<int> v{1, 2, 3};
    EXPECT_EQ(v.at(1), 2);
}

// Verifies at() throws for an out-of-range index.
TEST(Access, AtOutOfRangeThrows) {
    Vector<int> v{1, 2, 3};
    EXPECT_THROW((void)v.at(10), std::out_of_range);
}

// Verifies front() returns the first element.
TEST(Access, FrontBasic) {
    Vector<int> v{5, 6, 7};
    EXPECT_EQ(v.front(), 5);
}

// Verifies back() returns the last element.
TEST(Access, BackBasic) {
    Vector<int> v{5, 6, 7};
    EXPECT_EQ(v.back(), 7);
}

// Verifies front() and back() return the same element for a single-element vector.
TEST(Access, FrontBackSingleElement) {
    Vector<int> v{42};
    EXPECT_EQ(v.front(), 42);
    EXPECT_EQ(v.back(), 42);
}

// Verifies operator[] provides read access to elements.
TEST(Access, OperatorIndexRead) {
    Vector<int> v{1, 2, 3};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[2], 3);
}

// Verifies operator[] provides writable access to elements.
TEST(Access, OperatorIndexWrite) {
    Vector<int> v{1, 2, 3};
    v[1] = 99;
    EXPECT_EQ(v[1], 99);
}

// Verifies all const accessors return the expected values.
TEST(Access, ConstAccess) {
    const Vector<int> v{1, 2, 3};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
    EXPECT_EQ(v.at(1), 2);
}
