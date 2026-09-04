# Fuzzing

VectorPro is fuzzed via [ClusterFuzzLite](https://google.github.io/clusterfuzzlite/),
running on every pull request that touches the fuzzed files, plus a
longer scheduled batch run every night.

## What's covered

**`fuzz_vector.cpp`** is a differential fuzzer: it runs the same
sequence of operations against `Vector<int>` and a `std::vector<int>`
shadow model, comparing size and contents after every single
operation (not just at the end), so a failing input localizes to the
exact operation that broke an invariant.

`int` is used deliberately, not for simplicity — it's trivially
copyable, which routes every operation through Vector's memcpy-based
fast paths (`reallocate()`, `copyBufferFrom()`, copy assignment,
`operator==`) rather than the general per-element path. That's exactly
where a subtle off-by-one or aliasing bug in the optimized code would
hide, and exactly what this harness is trying to catch.

Specifically exercised:

- Growth and reallocation correctness across several starting
  capacities, biased toward small ones so most runs force at least
  one `Buffer`-style reallocation rather than only exercising a
  comfortably pre-sized buffer.
- The **aliasing-safe push_back/insert path** — pushing or inserting a
  reference to an element already inside the same vector
  (`v.push_back(v[i])`), which is one of VectorPro's distinguishing
  features and not something a generic vector fuzzer would think to
  target.
- `at()`'s bounds-checking contract: the harness deliberately picks
  out-of-range indices roughly half the time and confirms
  `std::out_of_range` is thrown exactly when, and only when, the index
  is actually out of range.
- **Both `operator=` overloads (copy and move), including
  self-assignment.** These are the two functions in the class with the
  highest cognitive-complexity findings from static analysis,
  deliberately left un-refactored — see the `NOLINT` comments in
  `Vector.tpp` — because splitting them risked introducing a real bug
  in exception-sensitive code for no correctness gain. This harness is
  the automated safety net that decision leans on instead of a manual
  refactor: if growth/allocator-propagation logic in either operator
  is ever wrong, this is what's meant to catch it.

Built and run under both AddressSanitizer and UndefinedBehaviorSanitizer.

## What's deliberately NOT covered yet

- **Custom allocators.** Only the default `std::allocator` is
  exercised. A stateful test allocator that fuzzes
  `propagate_on_container_copy_assignment`/`..._move_assignment`/
  `select_on_container_copy_construction` combinations would be a
  natural follow-up harness, not a change to this one.
- **The observer/event-notification system** (`ObservableVector`,
  `subscribe()`/`unsubscribe()`).
- **Exception injection during copy construction/assignment.** This
  harness never makes an operation throw, so it never actually
  exercises the strong-exception-guarantee rollback paths in
  `reallocate()`/`copyBufferFrom()`'s catch blocks — only their
  non-throwing path. A throwing test element type (throws on the Nth
  construction/copy) is what a follow-up harness would need to
  actually stress those catch blocks, which is a different, more
  invasive harness design than differential comparison against
  `std::vector`.

## Running locally

```bash
git clone --recursive https://github.com/google/oss-fuzz.git
cd oss-fuzz
python infra/helper.py build_fuzzers --sanitizer address VectorPro /path/to/VectorPro
python infra/helper.py run_fuzzer VectorPro fuzz_vector
```

Or, without OSS-Fuzz's tooling, directly with clang:

```bash
clang++ -std=c++20 -fsanitize=fuzzer,address \
  -Iinclude \
  fuzz/fuzz_vector.cpp \
  -o fuzz_vector

./fuzz_vector
```

Add `-fsanitize=fuzzer,undefined` instead to run under UBSan.

## Reproducing a crash

ClusterFuzzLite uploads the failing input as a workflow artifact when
a run fails. Download it, then:

```bash
./fuzz_vector path/to/crash-<hash>
```

This replays that exact byte sequence through
`LLVMFuzzerTestOneInput()` once, deterministically — no sanitizer flags
needed beyond however the binary was already built.

## Adding a new harness

1. Add `fuzz/fuzz_<target>.cpp` with an `extern "C" int
   LLVMFuzzerTestOneInput(const uint8_t*, size_t)` entry point.
2. Add the matching compile + link block to `.clusterfuzzlite/build.sh`.
3. No workflow changes needed — `cflite_pr.yml`/`cflite_batch.yml`
   build and run every binary `build.sh` produces in `$OUT`.
