# Benchmark Suite

This document describes the benchmark categories under `suite/` — what
each one measures, and the individual benchmarks it contains.

Every benchmark compares VectorPro's `Vector` against `std::vector` —
the standard's dynamic array, and the natural baseline for a drop-in
replacement. A category can support more than one standard for
comparison, but for now each category is benchmarked against a single
standard.

Every `BENCH()` call, in every category below, is automatically repeated
at three iteration tiers — SMALL (10K), MEDIUM (100K), and LARGE (1M) —
to smooth out timing noise and show whether relative performance holds
steady as call volume increases. This applies uniformly across the whole
suite; it is not specific to any one category. The **Scaling** category
below measures something different: how per-operation cost changes as
capacity itself grows or shrinks, independent of iteration count.

Some benchmarks have no meaningful std::vector equivalent — std::vector
provides no modification-event notification mechanism. Those run
through `BENCH_SOLO()` instead of `BENCH()`, timing Vector alone.

---

## Access

Benchmarks read and lookup operations on an already-populated vector —
element access, traversal, and linear search.

### Benchmarks

- `element_access.cpp` — `at()` bounds-checked access, `front()`/`back()`,
  raw pointer access via `data_ptr()` / `data()`
- `iteration.cpp` — range-for, forward iterator loop, index-based
  (`operator[]`) loop, reverse iteration, `std::accumulate`
- `search.cpp` — `contains()` hit (small/large), `contains()` miss,
  `find()` hit, `find()` miss

---

## Core

Benchmarks the fundamental, most frequently exercised operations —
inserting, appending, erasing, removing, and clearing elements.

### Benchmarks

- `insert.cpp` — `insert()` at front, middle, and end
- `push_back.cpp` — `push_back()`, `push_back()` with reserved capacity,
  `emplace_back()`
- `erase.cpp` — `erase()` at front, middle, and end
- `pop_clear.cpp` — `pop_back()` down to empty, `clear()` on a full vector
- `remove_if.cpp` — `remove_if()` against std::vector's erase-remove
  idiom

---

## Lifecycle

Benchmarks object lifetime operations — construction, copying, and
moving.

### Benchmarks

- `construction.cpp` — default construction, fill construction,
  initializer list construction
- `copy.cpp` — copy construction, copy assignment (reusable destination
  capacity)
- `move.cpp` — move construction, move assignment

---

## Scaling

Benchmarks how per-operation cost changes as capacity itself grows or
shrinks — a separate axis from the SMALL/MEDIUM/LARGE iteration tiers
described above: those repeat the same fixed-size operation more times,
while Scaling grows or shrinks the vector itself and observes the
resulting cost.

### Benchmarks

- `reallocation.cpp` — single growth-triggered reallocation on a
  populated buffer, isolating move/copy-and-free cost from `push_back()`
  itself
- `reserve.cpp` — `reserve()` on an empty vector, repeated growth
  targets, no-op reserve (capacity already sufficient)
- `shrink.cpp` — `shrink_to_fit()` after over-reserving capacity

---

## Utility

Benchmarks bookkeeping and comparison operations that don't belong to
any of the categories above — equality/ordering and swap semantics, plus
event-notification overhead unique to Vector.

### Benchmarks

- `comparison.cpp` — `operator==` (equal, differing early, differing
  late, different sizes), `operator<=>` (equal, less)
- `observer.cpp` — `push_back()` with 0/1/8 listeners, `push_back()`
  with a capturing listener, `subscribe()`, `unsubscribe()` (solo, no
  std::vector equivalent)
- `swap.cpp` — member `swap()`, free-function `swap()` (ADL)
