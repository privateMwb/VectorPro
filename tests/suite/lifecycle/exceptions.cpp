// Vector Exception Safety Test Suite
// Verifies the vector's behavior when element construction, copying, or
// moving throws mid-operation, using an instrumented type that can be told
// to fail on demand.
//
// Covers:
// - copy construction throws and leaves the source untouched
// - copy construction throws without leaking already-copied elements
// - copy assignment (grow path) throws and preserves the target (strong guarantee)
// - copy assignment (fits path) throws but leaves size at its pre-call value (basic guarantee)
// - push_back-triggered reallocation throws and preserves the original vector
// - emplace_back constructor throws without incrementing size

#include <support/framework.h>

using namespace VectorPro;

namespace {

// A type that can be configured to throw on its Nth copy/move/construction.
// `throwAfter == -1` means "never throw". The counter is shared across all
// instances so tests can simulate failure partway through a bulk operation.
struct Faulty {
    static inline int throwAfter = -1;
    static inline int counter = 0;
    static inline int alive = 0;

    int value;

    explicit Faulty(int v = 0) : value(v) {
        if (throwAfter >= 0 && counter++ >= throwAfter)
            throw std::runtime_error("Faulty: forced construction failure");
        ++alive;
    }

    Faulty(const Faulty& other) : value(other.value) {
        if (throwAfter >= 0 && counter++ >= throwAfter)
            throw std::runtime_error("Faulty: forced copy failure");
        ++alive;
    }

    // Deliberately NOT noexcept: this makes std::move_if_noexcept (used by
    // Vector::reallocate) fall back to the copy constructor, which is the
    // scenario being exercised in the reallocation test below.
    Faulty(Faulty&& other) : value(other.value) {
        if (throwAfter >= 0 && counter++ >= throwAfter)
            throw std::runtime_error("Faulty: forced move failure");
        ++alive;
    }

    Faulty& operator=(const Faulty& other) {
        if (throwAfter >= 0 && counter++ >= throwAfter)
            throw std::runtime_error("Faulty: forced copy-assign failure");
        value = other.value;
        return *this;
    }

    Faulty& operator=(Faulty&& other) noexcept {
        value = other.value;
        return *this;
    }

    ~Faulty() {
        --alive;
    }
};

// Resets Faulty's shared state. Called at the start of every test.
void reset_faulty() {
    Faulty::throwAfter = -1;
    Faulty::counter = 0;
    Faulty::alive = 0;
}

} // namespace

// Verifies a throwing copy constructor during Vector(const Vector&) leaves
// the source completely untouched.
static void copy_construction_throw_leaves_source_untouched() {
    reset_faulty();

    Vector<Faulty> source;
    source.push_back(Faulty(1));
    source.push_back(Faulty(2));
    source.push_back(Faulty(3));
    source.push_back(Faulty(4));
    source.push_back(Faulty(5));

    Faulty::counter = 0;
    Faulty::throwAfter = 2; // Throws on the 3rd copy, mid-construction.

    CHK_THROWS((Vector<Faulty>(source)), std::runtime_error);

    Faulty::throwAfter = -1;
    CHK(source.size() == 5);
    for (std::size_t i = 0; i < source.size(); ++i) {
        CHK(source[i].value == static_cast<int>(i) + 1);
    }
}

// Verifies a throwing copy constructor during Vector(const Vector&) does not
// leak the elements that were already successfully copied before the throw.
static void copy_construction_throw_does_not_leak() {
    reset_faulty();

    Vector<Faulty> source;
    for (int i = 0; i < 5; ++i)
        source.push_back(Faulty(i));

    int aliveBeforeAttempt = Faulty::alive;

    Faulty::counter = 0;
    Faulty::throwAfter = 2;

    CHK_THROWS((Vector<Faulty>(source)), std::runtime_error);

    Faulty::throwAfter = -1;
    // Only the source's own elements should remain; every partial copy
    // made before the throw must have been destroyed during rollback.
    CHK(Faulty::alive == aliveBeforeAttempt);
}

// Verifies copy assignment's copy-and-swap fallback (target capacity too
// small) leaves the target fully unchanged when the copy throws (strong
// exception guarantee). The source must exceed INITIAL_CAP (8) so the
// target's default capacity is actually insufficient and the grow path
// (as opposed to the fits-in-place fast path) is the one under test.
static void copy_assignment_grow_path_throw_preserves_target() {
    reset_faulty();

    Vector<Faulty> target;
    target.push_back(Faulty(100));
    CHK(target.capacity() < 12); // sanity: confirms the grow path is reachable

    Vector<Faulty> source;
    for (int i = 0; i < 12; ++i)
        source.push_back(Faulty(i));

    Faulty::counter = 0;
    Faulty::throwAfter = 5;

    CHK_THROWS(target = source, std::runtime_error);

    Faulty::throwAfter = -1;
    CHK(target.size() == 1);
    CHK(target[0].value == 100);
}

// Verifies copy assignment's fast path (target capacity already sufficient)
// leaves size() at its pre-call value if the extension loop throws (basic
// guarantee, as documented in Vector.tpp).
static void copy_assignment_fits_path_throw_basic_guarantee() {
    reset_faulty();

    Vector<Faulty> target;
    target.reserve(10);
    target.push_back(Faulty(100));
    std::size_t sizeBefore = target.size();

    Vector<Faulty> source;
    for (int i = 0; i < 5; ++i)
        source.push_back(Faulty(i));

    Faulty::counter = 0;
    Faulty::throwAfter = 2;

    CHK_THROWS(target = source, std::runtime_error);

    Faulty::throwAfter = -1;
    CHK(target.size() == sizeBefore);
}

// Verifies that a push_back-triggered reallocation which throws mid-move
// leaves the original vector completely unchanged (strong guarantee).
static void push_back_reallocation_throw_preserves_original() {
    reset_faulty();

    Vector<Faulty> v;
    v.reserve(4);
    for (int i = 0; i < 4; ++i)
        v.push_back(Faulty(i));

    std::size_t sizeBefore = v.size();
    std::size_t capBefore = v.capacity();

    // vsize_ == vcap_, so the next push_back must reallocate; Faulty's move
    // ctor is not noexcept, so reallocate() falls back to copying existing
    // elements into the new buffer, and that copy is what we make fail.
    Faulty::counter = 0;
    Faulty::throwAfter = 1;

    CHK_THROWS(v.push_back(Faulty(99)), std::runtime_error);

    Faulty::throwAfter = -1;
    CHK(v.size() == sizeBefore);
    CHK(v.capacity() == capBefore);
    for (std::size_t i = 0; i < v.size(); ++i) {
        CHK(v[i].value == static_cast<int>(i));
    }
}

// Verifies emplace_back does not increment size() if T's constructor throws.
static void emplace_back_constructor_throw_does_not_grow_size() {
    reset_faulty();

    Vector<Faulty> v;
    v.push_back(Faulty(1));
    v.push_back(Faulty(2));
    std::size_t sizeBefore = v.size();

    Faulty::counter = 0;
    Faulty::throwAfter = 0; // The very next construction throws.

    CHK_THROWS(v.emplace_back(42), std::runtime_error);

    Faulty::throwAfter = -1;
    CHK(v.size() == sizeBefore);
}

// Executes all exception-safety test cases.
static void run_tests() {
    RUN(copy_construction_throw_leaves_source_untouched);
    RUN(copy_construction_throw_does_not_leak);
    RUN(copy_assignment_grow_path_throw_preserves_target);
    RUN(copy_assignment_fits_path_throw_basic_guarantee);
    RUN(push_back_reallocation_throw_preserves_original);
    RUN(emplace_back_constructor_throw_does_not_grow_size);
}

REGISTER_TEST_SUITE();
