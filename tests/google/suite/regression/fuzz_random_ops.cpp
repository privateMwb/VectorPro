// Vector Randomized Operation Fuzz Suite
// Drives a Vector through a long sequence of randomly chosen mutating
// operations (with a fixed seed for reproducibility) and cross-checks its
// observable state against a std::vector<int> reference model after every
// step. Also covers self-aliasing push_back/insert (e.g. v.push_back(v[i]))
// and a move-only element type, since those are the two classes of bug most
// likely to slip past hand-written unit tests but show up under sanitizers.
//
// Covers:
// - 5000-step randomized push_back/pop_back/insert/erase sequence, checked
//   against a std::vector<int> reference after every operation
// - push_back(v[i]) and push_back(std::move(v[i])) during growth, i.e. the
//   reference aliases storage that reallocate() is about to invalidate
// - move-only element type (std::unique_ptr<int>) surviving push_back,
//   move construction, and move assignment
//
// Intended to run under ASan/UBSan in CI (see .github/workflows/sanitizers.yml)
// where any out-of-bounds write, use-after-free, or leak in the paths above
// fails the build even if every value-level assertion below happens to pass.

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

#include <memory>
#include <random>
#include <vector>

using namespace VectorPro;

// Runs a fixed-seed sequence of random mutations against both a VectorPro
// and a std::vector<int>, asserting they stay in lockstep after every step.
// A fixed seed keeps this reproducible: a failure here always fails the
// same way, rather than flaking based on run-to-run RNG state.
TEST(FuzzRandomOps, MatchesReferenceModelOverFiveThousandSteps) {
    std::mt19937 rng(42);
    Vector<int> actual;
    std::vector<int> reference;

    for (int step = 0; step < 5000; ++step) {
        const int op = static_cast<int>(rng() % 4);

        if (op == 0) {
            int value = static_cast<int>(rng());
            actual.push_back(value);
            reference.push_back(value);
        } else if (op == 1 && !reference.empty()) {
            actual.pop_back();
            reference.pop_back();
        } else if (op == 2 && !reference.empty()) {
            std::size_t index = rng() % reference.size();
            (void)actual.erase(actual.begin() + static_cast<std::ptrdiff_t>(index));
            reference.erase(reference.begin() + static_cast<std::ptrdiff_t>(index));
        } else if (op == 3) {
            std::size_t index = reference.empty() ? 0 : rng() % reference.size();
            int value = static_cast<int>(rng());
            (void)actual.insert(actual.begin() + static_cast<std::ptrdiff_t>(index), value);
            reference.insert(reference.begin() + static_cast<std::ptrdiff_t>(index), value);
        }

        ASSERT_EQ(actual.size(), reference.size()) << "diverged at step " << step;
    }

    ASSERT_EQ(actual.size(), reference.size());
    for (std::size_t i = 0; i < reference.size(); ++i) {
        EXPECT_EQ(actual[i], reference[i]) << "value mismatch at index " << i;
    }
}

// Repeatedly appends the vector's own last element to itself across many
// growth reallocations. Each push_back(v.back()) passes a reference into
// the vector's own storage right as it's about to be reallocated, which is
// exactly the scenario grow_and_push_back()'s offset-snapshot exists to
// handle; this exercises it across dozens of reallocations rather than once.
TEST(FuzzRandomOps, SelfAliasingPushBackSurvivesManyReallocations) {
    Vector<int> v;
    v.push_back(1);

    for (int i = 0; i < 200; ++i) {
        v.push_back(v.back());
    }

    ASSERT_EQ(v.size(), 201u);
    for (std::size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 1);
    }
}

// Same self-aliasing scenario, but via the move overload: push_back(std::
// move(v[i])) where the moved-from element lives in the buffer being
// reallocated. Confirms grow_and_push_back(T&&) reads the pre-move value
// from the *new* buffer's snapshot offset, not a dangling pointer into the
// freed old one.
TEST(FuzzRandomOps, SelfAliasingMovePushBackSurvivesManyReallocations) {
    Vector<int> v;
    v.push_back(1);

    for (int i = 0; i < 200; ++i) {
        v.push_back(std::move(v[v.size() - 1]));
    }

    ASSERT_EQ(v.size(), 201u);
    for (std::size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 1);
    }
}

// Move-only element type through push_back, move-construction of the whole
// vector, and move-assignment. std::unique_ptr<int> can't be copied, so any
// code path that silently assumes T is copyable (e.g. a stray construct()
// call using an lvalue) would fail to compile rather than fail at runtime,
// but this also checks ownership actually transfers rather than double-frees.
TEST(FuzzRandomOps, MoveOnlyElementSurvivesPushBackAndMove) {
    Vector<std::unique_ptr<int>> v;
    for (int i = 0; i < 50; ++i) {
        v.push_back(std::make_unique<int>(i));
    }

    Vector<std::unique_ptr<int>> moved = std::move(v);
    EXPECT_TRUE(v.empty());
    ASSERT_EQ(moved.size(), 50u);
    for (int i = 0; i < 50; ++i) {
        ASSERT_NE(moved[static_cast<std::size_t>(i)], nullptr);
        EXPECT_EQ(*moved[static_cast<std::size_t>(i)], i);
    }

    Vector<std::unique_ptr<int>> assigned;
    assigned = std::move(moved);
    EXPECT_TRUE(moved.empty());
    ASSERT_EQ(assigned.size(), 50u);
    EXPECT_EQ(*assigned[0], 0);
    EXPECT_EQ(*assigned[49], 49);
}
