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

#include <support/framework.h>

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
static void destructor_releases_all_elements() {
    reset_probe_counters();

    {
        Vector<Probe> v;
        v.push_back(Probe(1));
        v.push_back(Probe(2));
        v.push_back(Probe(3));
        CHK(Probe::alive == 3);
    }

    CHK(Probe::alive == 0);
}

// Verifies clear() destroys every element while the vector itself survives.
static void clear_destroys_all_elements() {
    reset_probe_counters();

    Vector<Probe> v;
    v.push_back(Probe(1));
    v.push_back(Probe(2));
    v.push_back(Probe(3));
    v.push_back(Probe(4));

    v.clear();

    CHK(Probe::alive == 0);
    CHK(v.size() == 0);
}

// Verifies pop_back() destroys exactly one element.
static void pop_back_destroys_one() {
    reset_probe_counters();

    Vector<Probe> v;
    v.push_back(Probe(1));
    v.push_back(Probe(2));
    v.push_back(Probe(3));

    int aliveBefore = Probe::alive;
    v.pop_back();

    CHK(Probe::alive == aliveBefore - 1);
}

// Verifies erase(pos) destroys exactly one element.
static void erase_single_destroys_one() {
    reset_probe_counters();

    Vector<Probe> v;
    v.push_back(Probe(1));
    v.push_back(Probe(2));
    v.push_back(Probe(3));

    int aliveBefore = Probe::alive;
    (void)v.erase(v.cbegin() + 1);

    CHK(Probe::alive == aliveBefore - 1);
}

// Verifies erase(first, last) destroys exactly the elements in the range.
static void erase_range_destroys_count() {
    reset_probe_counters();

    Vector<Probe> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(Probe(i));

    int aliveBefore = Probe::alive;
    (void)v.erase(v.cbegin() + 1, v.cbegin() + 4);

    CHK(Probe::alive == aliveBefore - 3);
    CHK(v.size() == 2);
}

// Verifies remove_if() destroys only the matched elements.
static void remove_if_destroys_matched_elements() {
    reset_probe_counters();

    Vector<Probe> v;
    for (int i = 0; i < 6; ++i)
        v.push_back(Probe(i));

    std::size_t removed = v.remove_if([](const Probe& p) { return p.value % 2 == 0; });

    CHK(removed == 3);
    CHK(Probe::alive == static_cast<int>(v.size()));
}

// Verifies the alive-object count always matches size(), even across the
// multiple reallocations triggered by sustained growth.
static void reallocate_preserves_alive_count() {
    reset_probe_counters();

    Vector<Probe> v;
    for (int i = 0; i < 200; ++i) {
        v.push_back(Probe(i));
        CHK(Probe::alive == static_cast<int>(v.size()));
    }
}

// Verifies copy construction produces fully independent live objects rather
// than sharing or double-counting the source's elements.
static void copy_construction_creates_independent_live_objects() {
    reset_probe_counters();

    Vector<Probe> a;
    a.push_back(Probe(1));
    a.push_back(Probe(2));
    a.push_back(Probe(3));

    int aliveAfterA = Probe::alive;
    Vector<Probe> b(a);

    CHK(Probe::alive == aliveAfterA * 2);

    b[0].value = 99;
    CHK(a[0].value == 1);
}

// Verifies move construction transfers ownership at the buffer level without
// invoking any per-element copy or move constructor.
static void move_construction_does_not_touch_elements() {
    reset_probe_counters();

    Vector<Probe> a;
    a.push_back(Probe(1));
    a.push_back(Probe(2));
    a.push_back(Probe(3));

    int aliveBefore = Probe::alive;
    int copiesBefore = Probe::copyCount;
    int movesBefore = Probe::moveCount;

    Vector<Probe> b(std::move(a));

    CHK(Probe::alive == aliveBefore);
    CHK(Probe::copyCount == copiesBefore);
    CHK(Probe::moveCount == movesBefore);
    CHK(b.size() == 3);
}

// Verifies destructing a moved-from vector is safe and does not affect the
// alive count (the moved-from vector holds no elements to destroy).
static void destructing_moved_from_vector_is_safe() {
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

    CHK(Probe::alive == 2);
    CHK(b.size() == 2);
}

// Executes all destruction test cases.
static void run_tests() {
    RUN(destructor_releases_all_elements);
    RUN(clear_destroys_all_elements);
    RUN(pop_back_destroys_one);
    RUN(erase_single_destroys_one);
    RUN(erase_range_destroys_count);
    RUN(remove_if_destroys_matched_elements);
    RUN(reallocate_preserves_alive_count);
    RUN(copy_construction_creates_independent_live_objects);
    RUN(move_construction_does_not_touch_elements);
    RUN(destructing_moved_from_vector_is_safe);
}

REGISTER_TEST_SUITE();
