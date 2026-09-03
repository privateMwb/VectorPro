// ============================================================
// fuzz/fuzz_vector.cpp
//
// Differential fuzzer for VectorPro::Vector<int>, checked against a
// std::vector<int> shadow model after every single operation (not
// just at the end) so a fuzzer-found failure localizes to the exact
// operation that caused it.
//
// int is deliberately chosen, not for simplicity but because it's
// trivially copyable -- this routes every operation through Vector's
// memcpy-based fast paths (reallocate(), copyBufferFrom(), copy
// assignment, operator==) rather than the general per-element path,
// which is exactly where a subtle off-by-one or aliasing bug in the
// optimized code would hide.
//
// Specifically targets:
//   - growth/reallocation correctness across several starting
//     capacities, biased toward small ones so most runs actually
//     force Buffer growth rather than only exercising a comfortably
//     pre-sized buffer
//   - the aliasing-safe push_back/insert path (pushing/inserting a
//     reference to an element already inside the same vector)
//   - at()'s bounds-checking contract (throws std::out_of_range,
//     exactly, exactly when the index is actually out of range)
//   - both operator= overloads (copy and move), including
//     self-assignment -- these are the two functions in the class
//     with the highest cognitive complexity, deliberately left
//     un-refactored (see the NOLINT comments in Vector.tpp) because
//     splitting them risked introducing a real bug in
//     exception-sensitive code for no correctness gain. This harness
//     is the automated safety net that decision relies on instead.
//
// Deliberately NOT covered yet: custom allocators (only the default
// std::allocator is exercised here), the observer/event-notification
// system (ObservableVector), and exception-injection during copy
// construction/assignment (would need a throwing test allocator or
// throwing element type to actually exercise the strong-guarantee
// rollback paths in reallocate()/copyBufferFrom()). That's the
// natural next harness to add here, not a replacement for this one.
// ============================================================

#include <VectorPro/Vector.h>

#include <cstdint>
#include <cstdlib>
#include <vector>

using VectorPro::Vector;

namespace {

// Aborts (rather than throwing/returning) on mismatch so libFuzzer
// captures a minimal, precise reproducer for exactly the operation
// that broke invariants.
void verify(const Vector<int>& v, const std::vector<int>& shadow) {
    if (v.size() != shadow.size())
        std::abort();

    for (std::size_t i = 0; i < shadow.size(); ++i) {
        if (v[i] != shadow[i])
            std::abort();
    }
}

} // namespace

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size) {
    if (size == 0)
        return 0;

    // First byte selects a starting capacity, biased toward small
    // values so most runs are forced through growth at least once.
    static constexpr std::size_t kCapacities[] = {0, 1, 2, 4, 64};
    const std::size_t capacity = kCapacities[data[0] % 5];
    ++data;
    --size;

    Vector<int> v;
    std::vector<int> shadow;
    if (capacity > 0) {
        v.reserve(capacity);
        shadow.reserve(capacity);
    }

    for (std::size_t i = 0; i < size; ++i) {
        const std::uint8_t op = data[i] % 13;

        switch (op) {
        case 0: { // push_back
            const int value = static_cast<int>(i);
            v.push_back(value);
            shadow.push_back(value);
            break;
        }
        case 1: { // emplace_back
            const int value = static_cast<int>(i);
            v.emplace_back(value);
            shadow.emplace_back(value);
            break;
        }
        case 2: { // insert(index, value)
            const int value = static_cast<int>(i);
            const std::size_t index = shadow.empty() ? 0 : (i % (shadow.size() + 1));
            v.insert(v.begin() + static_cast<std::ptrdiff_t>(index), value);
            shadow.insert(shadow.begin() + static_cast<std::ptrdiff_t>(index), value);
            break;
        }
        case 3: { // erase(index)
            if (!shadow.empty()) {
                const std::size_t index = i % shadow.size();
                v.erase(v.begin() + static_cast<std::ptrdiff_t>(index));
                shadow.erase(shadow.begin() + static_cast<std::ptrdiff_t>(index));
            }
            break;
        }
        case 4: { // pop_back
            if (!shadow.empty()) {
                v.pop_back();
                shadow.pop_back();
            }
            break;
        }
        case 5: { // clear
            v.clear();
            shadow.clear();
            break;
        }
        case 6: { // reserve
            const std::size_t extra = (i % 8);
            v.reserve(v.size() + extra);
            shadow.reserve(shadow.size() + extra);
            break;
        }
        case 7: { // shrink_to_fit
            v.shrink_to_fit();
            shadow.shrink_to_fit();
            break;
        }
        case 8: { // self-referencing push_back -- the aliasing-safety path
            if (!shadow.empty()) {
                const std::size_t index = i % shadow.size();
                v.push_back(v[index]);
                shadow.push_back(shadow[index]);
            }
            break;
        }
        case 9: { // self-referencing insert -- aliasing safety, insert variant
            if (!shadow.empty()) {
                const std::size_t srcIndex = i % shadow.size();
                const std::size_t destIndex = (i + 1) % (shadow.size() + 1);
                v.insert(v.begin() + static_cast<std::ptrdiff_t>(destIndex), v[srcIndex]);
                shadow.insert(shadow.begin() + static_cast<std::ptrdiff_t>(destIndex),
                              shadow[srcIndex]);
            }
            break;
        }
        case 10: { // at() bounds-checking contract
            // Roughly half the time, deliberately pick an out-of-range
            // index to confirm at() actually throws for it.
            const bool forceOutOfRange = (i % 2 == 0);
            const std::size_t index =
                forceOutOfRange ? shadow.size() + (i % 4) : (shadow.empty() ? 0 : i % shadow.size());

            bool threw = false;
            int value = 0;
            try {
                value = v.at(index);
            } catch (const std::out_of_range&) {
                threw = true;
            }

            const bool shouldThrow = index >= shadow.size();
            if (threw != shouldThrow)
                std::abort();
            if (!threw && value != shadow[index])
                std::abort();
            break;
        }
        case 11: { // copy assignment, including self-assignment
            const bool selfAssign = (i % 4 == 0);
            if (selfAssign) {
                Vector<int>& selfRef = v;
                v = selfRef;
                // shadow unchanged -- self-assignment is a no-op.
            } else {
                Vector<int> vCopy;
                std::vector<int> shadowCopy = shadow; // build an independent RHS
                for (int value : shadowCopy)
                    vCopy.push_back(value);

                v = vCopy;
                shadow = shadowCopy;
            }
            break;
        }
        case 12: { // move assignment, including self-move-assignment
            const bool selfMoveAssign = (i % 4 == 1);
            if (selfMoveAssign) {
                Vector<int>& selfRef = v;
                v = std::move(selfRef);
                // shadow unchanged -- moving from self must be a no-op,
                // not a self-destructive one.
            } else {
                Vector<int> vSource;
                std::vector<int> shadowSource = shadow;
                for (int value : shadowSource)
                    vSource.push_back(value);

                v = std::move(vSource);
                shadow = shadowSource;
            }
            break;
        }
        default:
            break;
        }

        verify(v, shadow);
    }

    return 0;
}
