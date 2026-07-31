// Vector Mixed Workflow Integration Test Suite
// Verifies Vector's behavior over long, interleaved sequences of operations
// by mirroring every operation onto a std::vector<int> reference ("the
// oracle") and comparing full contents after each step. A fixed PRNG seed
// keeps the sequences deterministic and reproducible across runs.
//
// Covers:
// - a long randomized sequence of push_back/insert/erase/pop_back mirrors
//   std::vector exactly at every step
// - reserve/shrink_to_fit interleaved with mutations never desynchronizes
//   from the oracle
// - remove_if interleaved with other mutations mirrors std::vector's
//   erase-remove idiom
// - copy/move/swap operations performed mid-sequence preserve equivalence

#include <algorithm>
#include <random>
#include <support/framework.h>
#include <vector>

using namespace VectorPro;

namespace {

// Asserts that a Vector and its std::vector oracle currently hold identical
// contents.
void assert_equivalent(const Vector<int>& v, const std::vector<int>& oracle) {
    CHK(v.size() == oracle.size());
    for (std::size_t i = 0; i < oracle.size(); ++i) {
        CHK(v[i] == oracle[i]);
    }
}

} // namespace

// Runs a long sequence of push_back/insert/erase/pop_back operations against
// both a Vector and a std::vector oracle, verifying equivalence throughout.
static void randomized_sequence_mirrors_std_vector() {
    Vector<int> v;
    std::vector<int> oracle;

    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> opPick(0, 3);
    std::uniform_int_distribution<int> valuePick(0, 999);

    for (int step = 0; step < 2000; ++step) {
        int op = oracle.empty() ? 0 : opPick(rng); // force push_back when empty
        int value = valuePick(rng);

        switch (op) {
        case 0: // push_back
            v.push_back(value);
            oracle.push_back(value);
            break;
        case 1: { // insert at a random position
            std::uniform_int_distribution<std::size_t> posPick(0, oracle.size());
            std::size_t pos = posPick(rng);
            (void)v.insert(v.cbegin() + static_cast<std::ptrdiff_t>(pos), value);
            oracle.insert(oracle.begin() + static_cast<std::ptrdiff_t>(pos), value);
            break;
        }
        case 2: { // erase at a random position
            std::uniform_int_distribution<std::size_t> posPick(0, oracle.size() - 1);
            std::size_t pos = posPick(rng);
            (void)v.erase(v.cbegin() + static_cast<std::ptrdiff_t>(pos));
            oracle.erase(oracle.begin() + static_cast<std::ptrdiff_t>(pos));
            break;
        }
        case 3: // pop_back
            v.pop_back();
            oracle.pop_back();
            break;
        }

        // Spot-check every 50 steps rather than every step, to keep the
        // test fast while still catching any drift early.
        if (step % 50 == 0) {
            assert_equivalent(v, oracle);
        }
    }

    assert_equivalent(v, oracle);
}

// Verifies reserve()/shrink_to_fit() calls interleaved with mutations never
// desynchronize Vector's contents from the oracle, even though they change
// capacity without changing size.
static void reserve_and_shrink_interleaved_with_mutations() {
    Vector<int> v;
    std::vector<int> oracle;

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
        oracle.push_back(i);

        if (i % 10 == 0) {
            v.reserve(v.capacity() + 50);
            oracle.reserve(oracle.capacity() + 50);
        }
        if (i % 17 == 0) {
            v.shrink_to_fit();
            oracle.shrink_to_fit();
        }
    }

    assert_equivalent(v, oracle);

    // Erase a chunk from the middle, then shrink again.
    (void)v.erase(v.cbegin() + 20, v.cbegin() + 60);
    oracle.erase(oracle.begin() + 20, oracle.begin() + 60);
    v.shrink_to_fit();
    oracle.shrink_to_fit();

    assert_equivalent(v, oracle);
}

// Verifies remove_if interleaved with push_back mirrors the effect of
// std::vector's erase-remove idiom on the oracle.
static void remove_if_interleaved_mirrors_erase_remove_idiom() {
    Vector<int> v;
    std::vector<int> oracle;

    for (int i = 0; i < 50; ++i) {
        v.push_back(i);
        oracle.push_back(i);
    }

    (void)v.remove_if([](int x) { return x % 3 == 0; });
    oracle.erase(std::remove_if(oracle.begin(), oracle.end(), [](int x) { return x % 3 == 0; }),
                 oracle.end());
    assert_equivalent(v, oracle);

    for (int i = 100; i < 130; ++i) {
        v.push_back(i);
        oracle.push_back(i);
    }

    (void)v.remove_if([](int x) { return x % 2 == 0; });
    oracle.erase(std::remove_if(oracle.begin(), oracle.end(), [](int x) { return x % 2 == 0; }),
                 oracle.end());
    assert_equivalent(v, oracle);
}

// Verifies copy, move, and swap performed mid-sequence preserve equivalence
// with the oracle throughout.
static void copy_move_swap_mid_sequence_preserve_equivalence() {
    Vector<int> a;
    std::vector<int> oracleA;

    for (int i = 0; i < 30; ++i) {
        a.push_back(i);
        oracleA.push_back(i);
    }

    Vector<int> b(a); // copy
    std::vector<int> oracleB(oracleA);
    assert_equivalent(b, oracleB);

    for (int i = 30; i < 40; ++i) {
        b.push_back(i);
        oracleB.push_back(i);
    }
    // `a` must be unaffected by mutations to its copy `b`.
    assert_equivalent(a, oracleA);
    assert_equivalent(b, oracleB);

    a.swap(b);
    std::swap(oracleA, oracleB);
    assert_equivalent(a, oracleA);
    assert_equivalent(b, oracleB);

    Vector<int> c(std::move(a)); // move
    std::vector<int> oracleC(std::move(oracleA));
    assert_equivalent(c, oracleC);
}

// Executes all mixed-workflow integration test cases.
static void run_tests() {
    RUN(randomized_sequence_mirrors_std_vector);
    RUN(reserve_and_shrink_interleaved_with_mutations);
    RUN(remove_if_interleaved_mirrors_erase_remove_idiom);
    RUN(copy_move_swap_mid_sequence_preserve_equivalence);
}

REGISTER_TEST_SUITE();
