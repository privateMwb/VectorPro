// Vector Destruction Test Suite
// Verifies construct/destroy balance across mutating operations, using an
// instrumented probe type since plain `int` cannot reveal this behavior.
//
// Covers:
// - vector destructor releases all elements
// - clear destroys all elements
// - pop_back destroys exactly one element
// - erase destroys exactly one element
// - erase range destroys the correct count
// - remove_if destroys only matched elements
// - reallocation preserves the alive-object invariant
// - copy construction produces independent live objects
// - move construction does not copy or move individual elements
// - destructing a moved-from vector is safe

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

namespace {

// Tracks construction/destruction/copy/move activity for every instance,
// so tests can verify the vector neither leaks nor double-destroys elements.
struct Probe {
    static inline int alive = 0;
    static inline int constructedCount = 0;
    static inline int destructedCount = 0;
    static inline int copyCount = 0;
    static inline int moveCount = 0;

    int value;

    explicit Probe(int v = 0) : value(v) {
        ++alive;
        ++constructedCount;
    }

    Probe(const Probe& other) : value(other.value) {
        ++alive;
        ++constructedCount;
        ++copyCount;
    }

    Probe(Probe&& other) noexcept : value(other.value) {
        ++alive;
        ++constructedCount;
        ++moveCount;
    }

    Probe& operator=(const Probe& other) {
        value = other.value;
        ++copyCount;
        return *this;
    }

    Probe& operator=(Probe&& other) noexcept {
        value = other.value;
        ++moveCount;
        return *this;
    }

    ~Probe() {
        --alive;
        ++destructedCount;
    }
};

// Resets all Probe counters. Called at the start of every test since the
// counters are static and shared across the whole suite.
void reset_probe_counters() {
    Probe::alive = 0;
    Probe::constructedCount = 0;
    Probe::destructedCount = 0;
    Probe::copyCount = 0;
    Probe::moveCount = 0;
}

} // namespace

// Verifies the vector's destructor destroys every remaining element.
TEST(Destruction, DestructorReleasesAllElements) {
    reset_probe_counters();

    {
        Vector<Probe> v;
        v.push_back(Probe(1));
        v.push_back(Probe(2));
        v.push_back(Probe(3));
        EXPECT_EQ(Probe::alive, 3);
    }

    EXPECT_EQ(Probe::alive, 0);
}

// Verifies clear() destroys every element while the vector itself survives.
TEST(Destruction, ClearDestroysAllElements) {
    reset_probe_counters();

    Vector<Probe> v;
    v.push_back(Probe(1));
    v.push_back(Probe(2));
    v.push_back(Probe(3));
    v.push_back(Probe(4));

    v.clear();

    EXPECT_EQ(Probe::alive, 0);
    EXPECT_EQ(v.size(), 0u);
}

// Verifies pop_back() destroys exactly one element.
TEST(Destruction, PopBackDestroysOne) {
    reset_probe_counters();

    Vector<Probe> v;
    v.push_back(Probe(1));
    v.push_back(Probe(2));
    v.push_back(Probe(3));

    int aliveBefore = Probe::alive;
    v.pop_back();

    EXPECT_EQ(Probe::alive, aliveBefore - 1);
}

// Verifies erase(pos) destroys exactly one element.
TEST(Destruction, EraseSingleDestroysOne) {
    reset_probe_counters();

    Vector<Probe> v;
    v.push_back(Probe(1));
    v.push_back(Probe(2));
    v.push_back(Probe(3));

    int aliveBefore = Probe::alive;
    (void)v.erase(v.cbegin() + 1);

    EXPECT_EQ(Probe::alive, aliveBefore - 1);
}

// Verifies erase(first, last) destroys exactly the elements in the range.
TEST(Destruction, EraseRangeDestroysCount) {
    reset_probe_counters();

    Vector<Probe> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(Probe(i));

    int aliveBefore = Probe::alive;
    (void)v.erase(v.cbegin() + 1, v.cbegin() + 4);

    EXPECT_EQ(Probe::alive, aliveBefore - 3);
    EXPECT_EQ(v.size(), 2u);
}

// Verifies remove_if() destroys only the matched elements.
TEST(Destruction, RemoveIfDestroysMatchedElements) {
    reset_probe_counters();

    Vector<Probe> v;
    for (int i = 0; i < 6; ++i)
        v.push_back(Probe(i));

    std::size_t removed = v.remove_if([](const Probe& p) { return p.value % 2 == 0; });

    EXPECT_EQ(removed, 3u);
    EXPECT_EQ(Probe::alive, static_cast<int>(v.size()));
}

// Verifies the alive-object count always matches size(), even across the
// multiple reallocations triggered by sustained growth.
TEST(Destruction, ReallocatePreservesAliveCount) {
    reset_probe_counters();

    Vector<Probe> v;
    for (int i = 0; i < 200; ++i) {
        v.push_back(Probe(i));
        EXPECT_EQ(Probe::alive, static_cast<int>(v.size()));
    }
}

// Verifies copy construction produces fully independent live objects rather
// than sharing or double-counting the source's elements.
TEST(Destruction, CopyConstructionCreatesIndependentLiveObjects) {
    reset_probe_counters();

    Vector<Probe> a;
    a.push_back(Probe(1));
    a.push_back(Probe(2));
    a.push_back(Probe(3));

    int aliveAfterA = Probe::alive;
    Vector<Probe> b(a);

    EXPECT_EQ(Probe::alive, aliveAfterA * 2);

    b[0].value = 99;
    EXPECT_EQ(a[0].value, 1);
}

// Verifies move construction transfers ownership at the buffer level without
// invoking any per-element copy or move constructor.
TEST(Destruction, MoveConstructionDoesNotTouchElements) {
    reset_probe_counters();

    Vector<Probe> a;
    a.push_back(Probe(1));
    a.push_back(Probe(2));
    a.push_back(Probe(3));

    int aliveBefore = Probe::alive;
    int copiesBefore = Probe::copyCount;
    int movesBefore = Probe::moveCount;

    Vector<Probe> b(std::move(a));

    EXPECT_EQ(Probe::alive, aliveBefore);
    EXPECT_EQ(Probe::copyCount, copiesBefore);
    EXPECT_EQ(Probe::moveCount, movesBefore);
    EXPECT_EQ(b.size(), 3u);
}

// Verifies destructing a moved-from vector is safe and does not affect the
// alive count (the moved-from vector holds no elements to destroy).
TEST(Destruction, DestructingMovedFromVectorIsSafe) {
    reset_probe_counters();

    Vector<Probe> b;
    {
        Vector<Probe> a;
        a.push_back(Probe(1));
        a.push_back(Probe(2));

        b = std::move(a);
        // `a` goes out of scope here; it should be empty and its
        // destructor should have nothing left to destroy.
    }

    EXPECT_EQ(Probe::alive, 2);
    EXPECT_EQ(b.size(), 2u);
}
