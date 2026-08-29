// Vector Modifiers Test Suite
// Verifies element insertion, removal, and capacity management.
//
// Covers:
// - push_back with lvalues
// - push_back with rvalues
// - push_back triggering reallocation
// - push_back with self-referenced elements
// - emplace_back
// - insert at the beginning
// - insert at the middle
// - insert at the end
// - insert with rvalues
// - insert with self-referenced elements
// - range insertion
// - emplace at arbitrary positions
// - remove_if with partial, no, and full matches
// - pop_back on populated and empty vectors
// - single-element erase
// - range erase
// - clear on populated and empty vectors
// - reserve growing capacity
// - reserve with a smaller requested capacity
// - shrink_to_fit

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies that push_back(const T&) appends and grows size correctly.
TEST(Modifiers, PushBackLvalue) {
    Vector<int> v;
    int x = 5;
    v.push_back(x);
    v.push_back(x);

    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 5);
}

// Verifies that push_back(T&&) moves the value in.
TEST(Modifiers, PushBackRvalue) {
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);

    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
}

// Verifies that repeated push_back triggers capacity growth without data loss.
TEST(Modifiers, PushBackGrowth) {
    Vector<int> v;
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    EXPECT_EQ(v.size(), 100u);
    EXPECT_GE(v.capacity(), 100u);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(v[i], i);
    }
}

// Verifies that push_back(v[i]) survives a reallocation triggered by the call.
TEST(Modifiers, PushBackSelfReference) {
    Vector<int> v(4, 0);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    v[3] = 4;
    // size == capacity, so this call forces reallocate() while `value`
    // still references the old (about-to-be-freed) buffer.
    v.push_back(v[0]);

    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[4], 1);
    EXPECT_EQ(v[0], 1);
}

// Verifies that emplace_back constructs the element in place with forwarded args.
TEST(Modifiers, EmplaceBackBasic) {
    Vector<int> v;
    v.emplace_back(42);
    v.emplace_back(43);

    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 42);
    EXPECT_EQ(v[1], 43);
}

// Verifies that insert(pos, value) at the end behaves like push_back.
TEST(Modifiers, InsertAtEnd) {
    Vector<int> v{1, 2, 3};
    auto it = v.insert(v.cend(), 4);

    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(*it, 4);
}

// Verifies that insert(pos, value) at the beginning shifts existing elements.
TEST(Modifiers, InsertAtBeginning) {
    Vector<int> v{1, 2, 3};
    auto it = v.insert(v.cbegin(), 0);

    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 3);
    EXPECT_EQ(*it, 0);
}

// Verifies that insert(pos, value) in the middle shifts only the tail.
TEST(Modifiers, InsertAtMiddle) {
    Vector<int> v{1, 2, 4, 5};
    auto it = v.insert(v.cbegin() + 2, 3);

    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
    EXPECT_EQ(*it, 3);
}

// Verifies that insert(pos, T&&) moves the value into place.
TEST(Modifiers, InsertRvalue) {
    Vector<int> v{1, 3};
    (void)v.insert(v.cbegin() + 1, 2);

    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1], 2);
}

// Verifies that insert(pos, v[i]) survives reallocation and shifting alike.
TEST(Modifiers, InsertSelfReference) {
    Vector<int> v(4, 0);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    v[3] = 4;
    // size == capacity: forces reallocate(); value also aliases the source buffer.
    (void)v.insert(v.cbegin() + 1, v[3]);

    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[1], 4);
}

// Verifies that insert(pos, first, last) inserts a full range at the given position.
TEST(Modifiers, InsertRange) {
    Vector<int> v{1, 2, 6};
    int src[] = {3, 4, 5};
    v.insert(v.cbegin() + 2, std::begin(src), std::end(src));

    EXPECT_EQ(v.size(), 6u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
    EXPECT_EQ(v[5], 6);
}

// Verifies that emplace(pos, args...) constructs in place at the given index.
TEST(Modifiers, EmplaceAtMiddle) {
    Vector<int> v{1, 2, 4};
    auto it = v.emplace(v.cbegin() + 2, 3);

    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(*it, 3);
}

// Verifies that emplace(pos, args...) at the end constructs directly without shifting.
TEST(Modifiers, EmplaceAtEnd) {
    Vector<int> v{1, 2};
    (void)v.emplace(v.cend(), 3);

    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[2], 3);
}

// Verifies that remove_if compacts the vector and returns the removed count.
TEST(Modifiers, RemoveIfBasic) {
    Vector<int> v{1, 2, 3, 4, 5, 6};
    std::size_t removed = v.remove_if([](const int& x) { return x % 2 == 0; });

    EXPECT_EQ(removed, 3u);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 5);
}

// Verifies that remove_if with no matches leaves the vector unchanged.
TEST(Modifiers, RemoveIfNoMatch) {
    Vector<int> v{1, 3, 5};
    std::size_t removed = v.remove_if([](const int& x) { return x % 2 == 0; });

    EXPECT_EQ(removed, 0u);
    EXPECT_EQ(v.size(), 3u);
}

// Verifies that remove_if matching everything empties the vector.
TEST(Modifiers, RemoveIfAllMatch) {
    Vector<int> v{2, 4, 6};
    std::size_t removed = v.remove_if([](const int&) { return true; });

    EXPECT_EQ(removed, 3u);
    EXPECT_EQ(v.size(), 0u);
}

// Verifies that pop_back removes the last element and shrinks size by one.
TEST(Modifiers, PopBackBasic) {
    Vector<int> v{1, 2, 3};
    v.pop_back();

    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

// Verifies that pop_back on an empty vector is a safe no-op.
TEST(Modifiers, PopBackEmpty) {
    Vector<int> v;
    v.pop_back();

    EXPECT_EQ(v.size(), 0u);
}

// Verifies that erase(pos) removes a single element and shifts the tail down.
TEST(Modifiers, EraseSingle) {
    Vector<int> v{1, 2, 3, 4};
    auto it = v.erase(v.cbegin() + 1);

    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 4);
    EXPECT_EQ(*it, 3);
}

// Verifies that erase(pos) on the last element leaves the tail iterator at end().
TEST(Modifiers, EraseLastElement) {
    Vector<int> v{1, 2, 3};
    auto it = v.erase(v.cbegin() + 2);

    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(it, v.end());
}

// Verifies that erase(first, last) removes a contiguous range.
TEST(Modifiers, EraseRange) {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.erase(v.cbegin() + 1, v.cbegin() + 4);

    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 5);
    EXPECT_EQ(*it, 5);
}

// Verifies that erase(first, last) with an empty range is a safe no-op.
TEST(Modifiers, EraseEmptyRange) {
    Vector<int> v{1, 2, 3};
    auto it = v.erase(v.cbegin() + 1, v.cbegin() + 1);

    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(*it, 2);
}

// Verifies that clear destroys all elements and resets size to zero.
TEST(Modifiers, ClearBasic) {
    Vector<int> v{1, 2, 3};
    std::size_t capBefore = v.capacity();
    v.clear();

    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), capBefore);
    EXPECT_TRUE(v.empty());
}

// Verifies that clear on an already-empty vector is a safe no-op.
TEST(Modifiers, ClearEmpty) {
    Vector<int> v;
    v.clear();

    EXPECT_EQ(v.size(), 0u);
}

// Verifies that reserve grows capacity without changing size or contents.
TEST(Modifiers, ReserveGrowsCapacity) {
    Vector<int> v{1, 2, 3};
    v.reserve(50);

    EXPECT_GE(v.capacity(), 50u);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

// Verifies that reserve with a smaller-than-current capacity is a no-op.
TEST(Modifiers, ReserveSmallerIsNoop) {
    Vector<int> v{1, 2, 3};
    v.reserve(50);
    std::size_t capAfterFirst = v.capacity();
    v.reserve(1);

    EXPECT_EQ(v.capacity(), capAfterFirst);
}

// Verifies that shrink_to_fit reduces capacity to match size.
TEST(Modifiers, ShrinkToFitBasic) {
    Vector<int> v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.shrink_to_fit();

    EXPECT_EQ(v.capacity(), v.size());
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

// Verifies that shrink_to_fit when already tight is a safe no-op.
TEST(Modifiers, ShrinkToFitAlreadyTight) {
    Vector<int> v{1, 2, 3};
    v.shrink_to_fit();

    EXPECT_EQ(v.capacity(), v.size());
    EXPECT_EQ(v.size(), 3u);
}
