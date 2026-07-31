// Vector Allocator Propagation Test Suite
// Verifies allocator-aware behavior using a custom, non-propagating,
// non-always-equal allocator whose activity is observable indirectly
// through per-tag allocation/deallocation counters (Vector exposes no
// get_allocator() accessor, so this is the only way to see which
// allocator instance actually did the work).
//
// Covers:
// - select_on_container_copy_construction copies the source's allocator
// - copy assignment never propagates the allocator (POCCA = false)
// - move assignment steals the buffer when allocators compare equal,
//   even though propagation is disabled
// - move assignment falls back to per-element moves under the target's
//   own allocator when allocators compare unequal and don't propagate
// - destructor deallocates under the vector's own allocator

#include <support/framework.h>
#include <unordered_map>

using namespace VectorPro;

namespace {

// Tracks allocate()/deallocate() calls per allocator tag.
struct AllocStats {
    static inline std::unordered_map<int, int> allocations;
    static inline std::unordered_map<int, int> deallocations;
};

void reset_alloc_stats() {
    AllocStats::allocations.clear();
    AllocStats::deallocations.clear();
}

// A minimal allocator identified by an integer tag. Two instances compare
// equal only if their tags match. Propagation is disabled on both copy and
// move assignment, and the allocator is not always-equal, so Vector must
// take the "manual" path in each case unless tags happen to match.
template <typename T> struct TaggedAllocator {
    using value_type = T;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using is_always_equal = std::false_type;

    int tag;

    explicit TaggedAllocator(int t = 0) noexcept : tag(t) {}

    template <typename U>
    TaggedAllocator(const TaggedAllocator<U>& other) noexcept : tag(other.tag) {}

    T* allocate(std::size_t n) {
        ++AllocStats::allocations[tag];
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) noexcept {
        ++AllocStats::deallocations[tag];
        ::operator delete(p);
    }

    friend bool operator==(const TaggedAllocator& a, const TaggedAllocator& b) noexcept {
        return a.tag == b.tag;
    }

    friend bool operator!=(const TaggedAllocator& a, const TaggedAllocator& b) noexcept {
        return !(a == b);
    }
};

} // namespace

// Verifies that copy-constructing a vector selects an allocator equivalent
// to the source's (default select_on_container_copy_construction behavior:
// the allocator itself is copy-constructed), observed indirectly via the
// tag under which the new buffer's allocation is recorded.
static void copy_construction_selects_source_allocator_tag() {
    reset_alloc_stats();

    Vector<int, TaggedAllocator<int>> a(TaggedAllocator<int>(5));
    a.push_back(1);
    a.push_back(2);

    CHK(AllocStats::allocations[5] > 0);
    int allocationsUnderTag5Before = AllocStats::allocations[5];

    Vector<int, TaggedAllocator<int>> b(a);

    // The copy's own allocation must have happened under tag 5, not some
    // other/default tag, confirming the source's allocator was propagated
    // into the copy via select_on_container_copy_construction.
    CHK(AllocStats::allocations[5] > allocationsUnderTag5Before);
    CHK(b.size() == 2);
}

// Verifies copy assignment never propagates the allocator (POCCA = false):
// the target keeps using its own allocator's tag for any new allocation,
// regardless of the source's tag.
static void copy_assignment_does_not_propagate_allocator() {
    reset_alloc_stats();

    Vector<int, TaggedAllocator<int>> target(TaggedAllocator<int>(1));
    target.push_back(0);

    Vector<int, TaggedAllocator<int>> source(TaggedAllocator<int>(2));
    for (int i = 0; i < 20; ++i)
        source.push_back(i); // forces target's capacity to be insufficient

    int allocationsUnderTag1Before = AllocStats::allocations[1];

    target = source;

    // The grow-path reallocation must have happened under the target's own
    // tag (1), never under the source's tag (2).
    CHK(AllocStats::allocations[1] > allocationsUnderTag1Before);
    CHK(target.size() == source.size());
}

// Verifies move assignment steals the source's buffer outright (no new
// allocation at all) when the two allocators compare equal, even though
// propagate_on_container_move_assignment is false for this allocator.
static void move_assignment_equal_allocators_steals_buffer() {
    reset_alloc_stats();

    Vector<int, TaggedAllocator<int>> target(TaggedAllocator<int>(7));
    target.push_back(-1);

    Vector<int, TaggedAllocator<int>> source(TaggedAllocator<int>(7)); // same tag
    for (int i = 0; i < 5; ++i)
        source.push_back(i);

    int allocationsBefore = AllocStats::allocations[7];

    target = std::move(source);

    // No new allocation should occur: the move assignment steals the
    // source's existing buffer since the allocators are equal.
    CHK(AllocStats::allocations[7] == allocationsBefore);
    CHK(target.size() == 5);
    CHK(target[0] == 0);
}

// Verifies move assignment falls back to allocating under the target's own
// allocator (and moving elements one by one) when the allocators compare
// unequal and neither propagates nor is always-equal.
static void move_assignment_unequal_allocators_uses_target_allocator() {
    reset_alloc_stats();

    Vector<int, TaggedAllocator<int>> target(TaggedAllocator<int>(10));
    target.push_back(-1);

    Vector<int, TaggedAllocator<int>> source(TaggedAllocator<int>(20)); // different tag
    for (int i = 0; i < 5; ++i)
        source.push_back(i);

    int allocationsUnderTargetTagBefore = AllocStats::allocations[10];

    target = std::move(source);

    // A fresh allocation must have happened under the target's own tag
    // (10), since the mismatched, non-propagating allocators rule out
    // simply stealing the source's buffer.
    CHK(AllocStats::allocations[10] > allocationsUnderTargetTagBefore);
    CHK(target.size() == 5);
    CHK(target[0] == 0);
}

// Verifies a vector's destructor deallocates its buffer under its own
// allocator's tag.
static void destructor_deallocates_under_own_allocator() {
    reset_alloc_stats();

    {
        Vector<int, TaggedAllocator<int>> v(TaggedAllocator<int>(3));
        v.push_back(1);
        v.push_back(2);
    }

    CHK(AllocStats::deallocations[3] > 0);
}

// Executes all allocator-propagation test cases.
static void run_tests() {
    RUN(copy_construction_selects_source_allocator_tag);
    RUN(copy_assignment_does_not_propagate_allocator);
    RUN(move_assignment_equal_allocators_steals_buffer);
    RUN(move_assignment_unequal_allocators_uses_target_allocator);
    RUN(destructor_deallocates_under_own_allocator);
}

REGISTER_TEST_SUITE();
