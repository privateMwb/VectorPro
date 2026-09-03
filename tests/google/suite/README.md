# Google Test Suite

This document describes the test categories under `suite/` — what each one
verifies, and the individual test files it contains. Same categories as
`../custom/suite/`, reimplemented with Google Test instead of the custom
framework.

| Category | Focus |
|---|---|
| [Concurrency](#concurrency) | Thread-safety across independent and shared-read instances |
| [Integration](#integration) | Multiple components working together end-to-end |
| [Lifecycle](#lifecycle) | Construction, destruction, copying, moving, allocators, exception safety |
| [Regression](#regression) | Previously fixed bugs that stay fixed |
| [Unit](#unit) | Individual functions or methods in isolation |
| [Conventions](#conventions) | Assertion style and macro gotchas specific to Google Test |

Unlike the benchmark suite, tests validate the library's own correctness
directly — there is no reference implementation to compare against, so
results are simply pass or fail.

Every test case auto-registers via `TEST(Suite, Case)` at startup — no
suite list, and no sequential id, to maintain by hand. Suite/case names
double as the filter you'd pass to `--gtest_filter`, e.g.
`--gtest_filter=Lifecycle*` runs every case in every `TEST(Lifecycle, ...)`
suite. This applies uniformly across every category below.

---

## Concurrency

Verifies thread-safety guarantees — that independent Vector instances never
interfere with one another, and that many threads reading a single shared
instance concurrently, with no writers, is safe.

### Tests

| File | What it covers |
|---|---|
| `isolation.cpp` | Many threads each building an independent Vector via push_back, a mixed insert/erase/sort sequence, and each own listener system never see another thread's data or notifications |
| `readonly.cpp` | Concurrent operator[] reads, iteration, find(), and contains() over one shared, unmutated Vector all agree and are correct |

---

## Integration

Verifies multiple components working together end-to-end — for example, a
full mixed-operation sequence mirrored step-by-step against a std::vector
oracle, rather than a single function in isolation.

### Tests

| File | What it covers |
|---|---|
| `workflow.cpp` | A long randomized push_back/insert/erase/pop_back sequence, reserve()/shrink_to_fit() interleaved with mutations, remove_if(), and copy/move/swap mid-sequence all mirror a std::vector oracle exactly |
| `ranges.cpp` | VectorPro::Iterator's std::contiguous_iterator conformance exercised end-to-end with std::sort/ranges::sort, std::find/ranges::find, std::reverse, std::accumulate, min/max_element, ranges::for_each, ranges::distance, and std::back_inserter |
| `integration_growth.cpp` | Custom GrowthNum/GrowthDen ratios (3/2, 4/3, 5/1) sustain thousands of push_backs with correct contents and monotonic capacity, amortized (not per-call) reallocation, and mixed push_back/insert/erase matching a default-ratio Vector |

---

## Lifecycle

Verifies object lifetime operations — construction, destruction, copying,
moving, allocator propagation, and exception safety.

### Tests

| File | What it covers |
|---|---|
| `construction.cpp` | Default, count+value, count+default-value, initializer list, iterator range, allocator, copy, copy assignment (including growth and self-assignment), move, move assignment, and self-move construction |
| `destruction.cpp` | Destructor, clear(), pop_back(), erase() (single and range), and remove_if() release exactly the right elements; reallocation preserves the alive-object invariant; copy/move construction and a moved-from vector's destruction are all safe |
| `allocators.cpp` | select_on_container_copy_construction on copy; copy assignment never propagates (POCCA = false); move assignment steals the buffer on equal allocators despite disabled propagation, and falls back to per-element moves under the target's own allocator when unequal; destructor deallocates under its own allocator |
| `exceptions.cpp` | Copy construction and copy assignment (grow and fits paths) throwing mid-operation preserve the strong or basic guarantee as documented; push_back-triggered reallocation and emplace_back throwing leave the vector unmodified |

---

## Regression

Verifies that a specific, previously fixed bug stays fixed. One test per
resolved issue, added at the time the fix lands.

### Tests

| File | What it covers |
|---|---|
| `handle.cpp` | unsubscribe()'s left-shift compaction strands a later handle out of range, lets its listener keep firing indefinitely, and can make a still-in-range handle remove the wrong listener |
| `regression_growth.cpp` | growCapacity()'s integer division could return the same capacity as before when starting from a small, non-INITIAL_CAP capacity under a fractional growth ratio, causing push_back() to write one element past the buffer |

---

## Unit

Verifies individual functions or methods in isolation — the smallest
testable unit of behavior, independent of the categories above.

### Tests

| File | What it covers |
|---|---|
| `access.cpp` | as_span() (including const and mutation-reflecting), data_ptr() (including empty), at() (valid and out-of-range), front()/back(), operator[] (read/write), const accessors |
| `capacity.cpp` | empty()/size() tracking across push_back/clear/pop_back, capacity() on default construction, capacity growth on overflow, capacity preserved after removal, growth-ratio conformance |
| `comparison.cpp` | operator== for identical/differing/different-sized/empty vectors, operator<=> for less-than, greater-than, equal, and shorter-prefix ordering |
| `iterators.cpp` | begin/end, const begin/end, cbegin/cend, rbegin/rend, crbegin/crend, range-for, iterator mutation, iterator-to-const_iterator conversion, random-access arithmetic and comparison, contiguous_iterator conformance |
| `modifiers.cpp` | push_back (lvalue, rvalue, growth-triggering, self-referenced), emplace_back, insert (front/middle/end, rvalue, self-referenced, range), emplace, remove_if (partial/none/all match), pop_back (populated/empty), erase (single/range), clear, reserve, shrink_to_fit |
| `observer.cpp` | subscribe() firing for every modifying operation with correct event payloads, multiple subscribers, unsubscribe() (valid, invalid handle, one of many), and operations with no subscribers |
| `search.cpp` | contains() (found/not found/empty), find() (mutable and const, hit/miss, first match on duplicates) |
| `swap.cpp` | member swap (contents, capacity, empty operand, self-swap), free swap (ADL) matching member swap, listeners transferred with swapped state |

---

## Conventions

- **Assertions** — `EXPECT_EQ`/`EXPECT_TRUE`/`EXPECT_FALSE` are used
  throughout, and `EXPECT_THROW(expr, ExceptionType)` for exception cases.
  Non-fatal (`EXPECT_*`) is used rather than fatal (`ASSERT_*`), so a single
  failed check in a test doesn't hide a second, independent failure later
  in the same test.
- **Comma-containing arguments** — an unparenthesized argument with a
  top-level comma (e.g. `std::vector<int>{3, 2, 1}`) will be misparsed by
  the `EXPECT_EQ(a, b)` macro as three arguments. Wrap it:
  `EXPECT_EQ(order, (std::vector<int>{3, 2, 1}))`.
- **`[[nodiscard]]` inside `EXPECT_THROW`/`EXPECT_NO_THROW`** — if the
  expression under test calls something `[[nodiscard]]`-marked without
  using the return value, the macro's expansion discards it and triggers a
  `-Wunused-result` warning. Cast it away explicitly:
  `EXPECT_THROW((void)v.at(10), std::out_of_range)`.
- **Signed/unsigned comparisons** — `size()`/`capacity()`-returning methods
  that return an unsigned type need a `u`-suffixed literal on the other
  side of `EXPECT_EQ` (e.g. `EXPECT_EQ(v.size(), 0u)`) to avoid a
  sign-compare warning.
