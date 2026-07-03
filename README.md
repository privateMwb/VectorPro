# VectorPro

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)](https://en.cppreference.com/w/cpp/23)
[![Status](https://img.shields.io/badge/status-learning%20project-green)](https://github.com/privateMwb/VectorPro)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A custom C++ dynamic array implementation built for learning contiguous storage
design, allocator-aware memory management, growth policies, and an observer
event system layered on top of `std::vector`-compatible semantics.

---

## Table of Contents

- [Overview](#overview)
- [Motivation / Goals](#motivation--goals)
- [Features](#features)
- [Design Overview](#design-overview)
- [Complexity](#complexity)
- [Quick Example](#quick-example)
- [Core API](#core-api)
- [Benchmark Results](#benchmark-results)
- [Project Structure](#project-structure)
- [Build Instructions](#build-instructions)
- [Notes](#notes)
- [License](#license)

---

## Overview

VectorPro (`Vector`) is a contiguous, `std::vector`-compatible dynamic array
implemented from scratch in modern C++ (C++23). It focuses on understanding
how growable contiguous containers work internally, including allocator-aware
allocation, trivial-type fast paths, exception-safe reallocation, and
iterator invalidation rules.

It also includes:

- Configurable growth policy via non-type template parameters
- Allocator awareness respecting `propagate_on_container_*` traits
- Trivially-copyable fast paths (`memcpy`) for the default allocator
- A built-in observer/event system via `subscribe()` / `unsubscribe()`
- Full `contiguous_iterator`-conforming iterator support
- Benchmark suite comparing against `std::vector` across every public operation
- Unit tests for correctness validation

---

## Motivation / Goals

This project was built to understand:

- Growth strategies for amortized O(1) `push_back`
- Allocator-aware construction, destruction, and propagation on copy/move
- Trivial vs non-trivial type dispatch (`memcpy` vs construct/destroy loops)
- Exception safety: strong guarantee via commit/rollback in `reallocate()`
- Iterator invalidation and `std::contiguous_iterator` conformance
- Self-referential argument hazards (`v.push_back(v[i])`) during reallocation
- Observer pattern design and its runtime cost when layered onto a hot path
- Performance benchmarking vs `std::vector` across construction, modification,
  search, iteration, and comparison

---

## Features

- Configurable growth ratio via `GrowthNum` / `GrowthDen` template parameters
- Allocator-aware copy/move construction and assignment
  (`select_on_container_copy_construction`, `propagate_on_container_move_assignment`)
- `memcpy` fast path for trivially-copyable types under the default allocator
- Exception-safe reallocation with rollback on a throwing move/copy constructor
- Self-reference safe `push_back` / `insert` (`v.push_back(v[i])` is well-defined)
- Copy assignment reuses existing capacity instead of always reallocating
- Observer system: `subscribe()` / `unsubscribe()` with typed `EventData` payloads
- `std::contiguous_iterator`-conforming `Iterator<T>`
- `std::span` interop via `as_span()`
- Three-way comparison (`operator<=>`) and equality (`operator==`)
- `[[no_unique_address]]` allocator storage — no size penalty for stateless allocators

---

## Design Overview

Vector uses a single contiguous heap-allocated buffer with size/capacity
tracking, plus a secondary buffer for optional event listeners.

### Internal Structure

```
data_ (pointer)
  ↓
[elem 0][elem 1][elem 2][...][elem n-1][  unused capacity  ]
                                        ↑
                                     vsize_
[                    vcap_                                 ]
```

- `alloc_`      → `[[no_unique_address]]` allocator instance
- `data_`       → pointer to the contiguous element buffer
- `vsize_`      → number of live elements
- `vcap_`       → total allocated capacity
- `listeners_`  → separate buffer of `std::function` event listeners
- `lsize_` / `lcap_` → listener count and listener buffer capacity

### Growth Strategy

Capacity grows by a configurable ratio (`GrowthNum / GrowthDen`, default `2/1`):

```cpp
Vector<int>                          v1;  // default 2/1 growth
Vector<int, std::allocator<int>, 3, 2> v2;  // conservative 3/2 growth
```

An overflow-guarded `growCapacity()` clamps to `SIZE_MAX` rather than wrapping
on very large capacities.

### Trivial-Type Fast Path

For trivially-copyable `T` with the default allocator, relocation during
`reallocate()` and copy construction uses `memcpy` instead of an
element-by-element construct/destroy loop:

```cpp
if constexpr (std::is_trivially_copyable_v<T> &&
              std::is_same_v<Allocator, std::allocator<T>>) {
    std::memcpy(std::to_address(newData), std::to_address(data_), vsize_ * sizeof(T));
} else {
    // move_if_noexcept construct/destroy loop, with rollback on exception
}
```

### Exception Safety Model

- `reallocate()` builds the new buffer fully before touching the old one —
  if a move/copy constructor throws mid-relocation, already-constructed
  elements in the new buffer are destroyed, the new buffer is freed, and the
  exception propagates with the original buffer untouched (strong guarantee).
- Copy assignment with sufficient existing capacity reuses storage in place
  and offers the *basic* guarantee (matching `std::vector`'s actual contract)
  — if an element's copy assignment throws mid-copy, `*this` remains valid
  but its exact contents are unspecified.
- Copy assignment that must grow falls back to copy-and-swap, preserving the
  strong guarantee.
- Move assignment and move construction are `noexcept`.

### Allocator Propagation

Copy and move assignment respect `propagate_on_container_copy_assignment`,
`propagate_on_container_move_assignment`, and `is_always_equal`. When
propagation doesn't hold and the allocators compare unequal, ownership is
never transferred across incompatible allocator instances — elements are
moved/copied individually instead.

### Self-Reference Safety

`push_back(v[i])` and `insert(pos, v[i])` are well-defined even when the call
triggers a reallocation that frees the buffer `v[i]` refers to. The source
index is captured before reallocation and re-resolved against the new buffer.

### Observer System

`subscribe()` registers a `std::function<void(const Vector&, EventData)>`
listener. Every mutating operation (`push_back`, `insert`, `erase`, `clear`,
`reserve`, etc.) fires a typed `EventData{ type, index, oldSize, newSize }`
to all subscribed listeners:

```cpp
auto handle = v.subscribe([](const Vector<int>& vec, Vector<int>::EventData e) {
    std::cout << "size " << e.oldSize << " -> " << e.newSize << "\n";
});

v.push_back(1);       // listener fires
v.unsubscribe(handle); // stops future notifications
```

This is the one part of the design with a measured, non-trivial runtime cost
— see [Benchmark Results](#benchmark-results).

---

## Complexity

### Time Complexity

| Operation                | Complexity      | Notes                                          |
| ------------------------ | --------------- | ----------------------------------------------- |
| `push_back` / `emplace_back` | O(1) amortized | May trigger `reallocate()`                   |
| `pop_back`                | O(1)            | Destructor call only                            |
| `insert` / `emplace`      | O(n)            | Shifts all elements after the insertion point   |
| `erase` (single)          | O(n)            | Shifts all elements after the erased index      |
| `erase` (range)           | O(n)            | Shifts the tail down by `count`                 |
| `remove_if`                | O(n)            | Single-pass compaction                          |
| `clear`                    | O(n)            | O(1) per element for trivially-destructible `T` |
| `reserve` / `shrink_to_fit`| O(n)            | One-time relocation, `memcpy` for trivial `T`   |
| `operator[]` / `at` / `front` / `back` | O(1) | Direct pointer offset                    |
| `contains` / `find`        | O(n)            | Linear scan                                     |
| `subscribe`                | O(1) amortized  | May grow the listener buffer                    |
| `unsubscribe`              | O(n)            | Shifts remaining listeners down                 |
| `swap`                     | O(1)            | Pointer/size swap only                          |

### Space Complexity

- O(n) for stored elements
- O(1) amortized wasted capacity relative to the configured growth ratio
- O(m) for `m` subscribed listeners (separate buffer from element storage)

---

## Quick Example

### Basic Usage

```cpp
#include "Vector.h"

using namespace VectorPro;

int main() {
    Vector<int> v{1, 2, 3};
    v.push_back(4);
    v.insert(v.cbegin(), 0);

    for (int x : v) { /* 0 1 2 3 4 */ }
}
```

### Observer Pattern

```cpp
#include "Vector.h"

using namespace VectorPro;

int main() {
    Vector<int> v;

    v.subscribe([](const Vector<int>&, Vector<int>::EventData e) {
        std::cout << "mutated: " << e.oldSize << " -> " << e.newSize << "\n";
    });

    v.push_back(1);   // fires listener
    v.pop_back();      // fires listener
}
```

### Custom Growth Policy

```cpp
#include "Vector.h"

using namespace VectorPro;

int main() {
    Vector<int, std::allocator<int>, 3, 2> v;   // grows by 1.5x instead of 2x
    for (int i = 0; i < 1000; ++i) v.push_back(i);
}
```

### Custom Allocator

```cpp
#include "Vector.h"
#include <memory_resource>

using namespace VectorPro;

int main() {
    std::byte buffer[4096];
    std::pmr::monotonic_buffer_resource resource(buffer, sizeof(buffer));
    std::pmr::polymorphic_allocator<int> alloc(&resource);

    Vector<int, std::pmr::polymorphic_allocator<int>> v(alloc);
    v.push_back(1);
}
```

---

## Core API

### Constructors

```cpp
Vector<T> v;                              // default construction
Vector<T> v(count, value);                // fill construction
Vector<T> v{1, 2, 3};                     // initializer list
Vector<T> v(first, last);                 // iterator range
Vector<T> v(alloc);                       // allocator construction
Vector<T> b(a);                           // copy construction
Vector<T> b(std::move(a));                // move construction
b = a;                                     // copy assignment
b = std::move(a);                         // move assignment
```

### Modifiers

```cpp
void push_back(const T& value);
void push_back(T&& value);

template<typename... Args> void emplace_back(Args&&... args);

[[nodiscard]] iterator insert(const_iterator pos, const T& value);
[[nodiscard]] iterator insert(const_iterator pos, T&& value);
template<std::input_iterator It> iterator insert(const_iterator pos, It first, It last);

template<typename... Args>
[[nodiscard]] iterator emplace(const_iterator pos, Args&&... args);

template<typename Predicate>
[[nodiscard]] std::size_t remove_if(Predicate pred);

void pop_back();
[[nodiscard]] iterator erase(const_iterator pos);
[[nodiscard]] iterator erase(const_iterator first, const_iterator last);

void clear() noexcept;
void reserve(std::size_t newCap);
void shrink_to_fit();
```

### Observer

```cpp
template<typename F>
[[nodiscard]] ListenerHandle subscribe(F&& listener);

void unsubscribe(ListenerHandle handle);
```

### Element Access

```cpp
[[nodiscard]] pointer data_ptr() noexcept;
[[nodiscard]] reference at(std::size_t index);
[[nodiscard]] reference front();
[[nodiscard]] reference back();
[[nodiscard]] reference operator[](std::size_t index) noexcept;

[[nodiscard]] std::span<T> as_span() noexcept;
```

### Search & Comparison

```cpp
[[nodiscard]] bool contains(const T& value) const;
[[nodiscard]] iterator find(const T& value);

[[nodiscard]] bool operator==(const Vector& other) const;
[[nodiscard]] auto operator<=>(const Vector& other) const;
```

### Capacity & Iterators

```cpp
[[nodiscard]] bool empty() const noexcept;
[[nodiscard]] std::size_t size() const noexcept;
[[nodiscard]] std::size_t capacity() const noexcept;

[[nodiscard]] iterator begin() noexcept;
[[nodiscard]] iterator end() noexcept;
[[nodiscard]] reverse_iterator rbegin() noexcept;
[[nodiscard]] reverse_iterator rend() noexcept;

void swap(Vector& other) noexcept;
```

---

## Benchmark Results

Benchmarks compare `Vector` against `std::vector<int>` across every public
operation. All times are total elapsed time for the listed iteration count.

> Compiled with `-std=c++23`, `-O3`. Results may vary depending on hardware
> and compiler optimizations.

### Constructor

```
----------------------------------------------------------------------
Constructor Benchmarks                  Time           Iteration
----------------------------------------------------------------------
VectorPro Default Construct             7.18 ms         1000000
Std::vector Default Construct           5.68 ms         1000000

VectorPro Fill Construct                81.68 ms        500000
Std::vector Fill Construct              75.51 ms        500000

VectorPro Init List Construct           65.68 ms        500000
Std::vector Init List Construct         67.08 ms        500000

VectorPro Copy Construct                321.73 ms       500000
Std::vector Copy Construct              351.60 ms       500000

VectorPro Move Construct                156.79 ms       500000
Std::vector Move Construct              122.89 ms       500000

VectorPro Copy Assignment               117.07 ms       500000
Std::vector Copy Assignment             108.77 ms       500000

VectorPro Move Assignment               87.52 ms        500000
Std::vector Move Assignment             165.68 ms       500000
----------------------------------------------------------------------
```

### Modifiers

```
----------------------------------------------------------------------
Modifiers Benchmarks                    Time           Iteration
----------------------------------------------------------------------
VectorPro Push Back                     8.22 s          500000
Std::vector Push Back                   2.05 s          500000

VectorPro Push Back Reserved            6.23 s          500000
Std::vector Push Back Reserved          898.74 ms       500000

VectorPro Emplace Back                  8.43 s          500000
Std::vector Emplace Back                1.58 s          500000

VectorPro Insert Front                  1.04 s          500000
Std::vector Insert Front                838.73 ms       500000

VectorPro Insert Middle                 888.29 ms       500000
Std::vector Insert Middle               816.43 ms       500000

VectorPro Insert End                    757.39 ms       500000
Std::vector Insert End                  770.14 ms       500000

VectorPro Erase Front                   298.69 ms       500000
Std::vector Erase Front                 292.77 ms       500000

VectorPro Erase Middle                  421.70 ms       500000
Std::vector Erase Middle                421.66 ms       500000

VectorPro Erase End                     296.77 ms       500000
Std::vector Erase End                   291.36 ms       500000

VectorPro Pop Back                      442.73 ms       500000
Std::vector Pop Back                    433.16 ms       500000

VectorPro Clear                         433.04 ms       500000
Std::vector Clear                       433.01 ms       500000

VectorPro Remove If                     14.21 s         500000
Std::vector Erase-remove If             3.28 s          500000
----------------------------------------------------------------------
```

### Capacity

```
----------------------------------------------------------------------
Capacity Benchmarks                     Time           Iteration
----------------------------------------------------------------------
VectorPro Reserve                       286.82 ms       500000
Std::vector Reserve                     282.58 ms       500000

VectorPro Reserve Growth                470.25 ms       100000
Std::vector Reserve Growth              463.32 ms       100000

VectorPro Shrink To Fit                 1.74 s          500000
Std::vector Shrink To Fit               647.18 ms       500000

VectorPro Growth Reallocation           1.39 s          500000
Std::vector Growth Reallocation         607.19 ms       500000

VectorPro Reserve No-op                 2.53 ms         1000000
Std::vector Reserve No-op               3.76 ms         1000000
----------------------------------------------------------------------
```

### Search

```
----------------------------------------------------------------------
Search Benchmarks                       Time           Iteration
----------------------------------------------------------------------
VectorPro Contains Hit (1k)             1.58 s          1000000
Std::vector Contains Hit (1k)           1.27 s          1000000

VectorPro Contains Miss (1k)            3.14 s          1000000
Std::vector Contains Miss (1k)          2.50 s          1000000

VectorPro Contains Hit (100k)           77.50 s         500000
Std::vector Contains Hit (100k)         75.51 s         500000

VectorPro Find Hit (1k)                 1.35 s          1000000
Std::vector Find Hit (1k)               1.36 s          1000000

VectorPro Find Miss (1k)                2.63 s          1000000
Std::vector Find Miss (1k)              2.49 s          1000000
----------------------------------------------------------------------
```

### Iteration

```
----------------------------------------------------------------------
Iteration Benchmarks                    Time           Iteration
----------------------------------------------------------------------
VectorPro Range-for                     39.04 s         500000
Std::vector Range-for                   39.14 s         500000

VectorPro Iterator Loop                 39.15 s         500000
Std::vector Iterator Loop               39.02 s         500000

VectorPro Index Loop                    39.14 s         500000
Std::vector Index Loop                  74.69 s         500000

VectorPro Reverse Iteration             69.41 s         500000
Std::vector Reverse Iteration           60.89 s         500000

VectorPro Accumulate                    39.21 s         500000
Std::vector Accumulate                  39.15 s         500000
----------------------------------------------------------------------
```

### Observer

```
----------------------------------------------------------------------
Observer Benchmarks                     Time           Iteration
----------------------------------------------------------------------
Push Back, 0 Listeners                  12.52 s         500000

Push Back, 1 Listener                   17.81 s         500000

Push Back, 8 Listeners                  51.42 s         500000

Push Back, Capturing Listener           29.77 s         500000

Subscribe X100                          5.92 s          500000

Unsubscribe X100                        50.13 s         500000
----------------------------------------------------------------------
```

### Comparison

```
----------------------------------------------------------------------
Comparison Benchmarks                   Time           Iteration
----------------------------------------------------------------------
VectorPro Operator== Equal              8.54 s          500000
Std::vector Operator== Equal            8.08 s          500000

VectorPro Operator== Differs Early      7.78 ms         500000
Std::vector Operator== Differs Early    6.85 ms         500000

VectorPro Operator== Differs Late       8.29 s          500000
Std::vector Operator== Differs Late     7.94 s          500000

VectorPro Operator== Diff Sizes         654.69 us       1000000
Std::vector Operator== Diff Sizes       1.23 ms         1000000

VectorPro Operator<=> Equal             16.85 s         500000
Std::vector Operator<=> Equal           28.02 s         500000

VectorPro Operator<=> Less              5.04 ms         500000
Std::vector Operator<=> Less            3.08 ms         500000
----------------------------------------------------------------------
```

### Summary

**Where VectorPro wins:**

- Copy construction (`321.73 ms` vs `351.60 ms`) benefits from the `memcpy`
  fast path for trivially-copyable types.
- Move assignment (`87.52 ms` vs `165.68 ms`) is nearly 2x faster — the
  allocator-propagation-aware steal path avoids per-element work entirely.
- `operator<=>` on equal-length equal-content vectors (`16.85 s` vs `28.02 s`)
  outperforms `std::vector` by a wide margin.
- Reserve no-op (`2.53 ms` vs `3.76 ms`) and index-loop iteration
  (`39.14 s` vs `74.69 s`) both favor VectorPro.
- `operator==` on different-sized vectors (`654.69 us` vs `1.23 ms`) — the
  immediate size check avoids any scan.

**Where VectorPro loses — known, unresolved:**

- `push_back` (`8.22 s` vs `2.05 s`) and `push_back` with pre-reserved
  capacity (`6.23 s` vs `898.74 ms`) are both significantly slower than
  `std::vector`, even with no reallocation involved. The most likely cause is
  the per-call self-reference aliasing check and the unconditional
  `EventData` construction + `notify()` call on every mutation, even with
  zero listeners subscribed.
- `emplace_back` (`8.43 s` vs `1.58 s`) and `remove_if`
  (`14.21 s` vs `3.28 s` for `std::vector`'s erase-remove idiom) show the
  same pattern.
- `shrink_to_fit` (`1.74 s` vs `647.18 ms`) and growth-triggered
  reallocation (`1.39 s` vs `607.19 ms`) are roughly 2x slower than
  `std::vector` despite the `memcpy` fast path — this needs further
  profiling to isolate whether the cost is in `notify()` overhead or
  elsewhere in `reallocate()`.
- The observer benchmarks confirm the notify path is not free even at zero
  listeners: `Push Back, 0 Listeners` (`12.52 s`) versus `Push Back, 1
  Listener` (`17.81 s`) shows real added cost per listener, and it compounds
  further at 8 listeners (`51.42 s`).

| Category                  | Winner        | Notes                                              |
| -------------------------- | ------------- | --------------------------------------------------- |
| Copy construction           | VectorPro     | `memcpy` fast path for trivial types                |
| Move assignment             | VectorPro     | Allocator-aware steal, no per-element work           |
| `operator<=>` (equal)        | VectorPro     | ~40% faster on full-length equal comparisons        |
| Index-loop iteration         | VectorPro     | ~2x faster than `std::vector` in this run            |
| `push_back` / `emplace_back` | std::vector   | 3-6x faster — event/notify overhead suspected        |
| `remove_if`                  | std::vector   | ~4x faster via erase-remove idiom                    |
| `shrink_to_fit` / reallocation | std::vector | ~2x faster despite matching fast path — needs profiling |
| Search, most iteration, erase | Tied         | Within noise of `std::vector`                        |

**Use VectorPro when:** you need the observer/event system, allocator
propagation correctness, or are working with trivially-copyable types where
the `memcpy` and move-assignment paths pay off.

**Use `std::vector` when:** `push_back`-heavy or `emplace_back`-heavy
workloads dominate and the event system isn't needed — the notify overhead
is currently a real, unresolved cost on the hottest path in the container.

---

## Project Structure

```
VectorPro/
├── include/
│   └── VectorPro/
│       ├── Iterator.h
│       ├── Vector.h
│       └── Vector.tpp
│
├── tests/
├── benchmarks/
├── examples/
│
├── cmake/
│   └── VectorProConfig.cmake.in
│
├── .gitignore
├── CMakeLists.txt
├── README.md
└── LICENSE
```

---

## Build Instructions

### Requirements

- GCC 16+ or Clang with C++23 support
- CMake 3.20+

### Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Run Tests

```bash
./tests
```

### Run Benchmarks

```bash
./benchmarks
```

### Run Examples

```bash
./example_basic
./example_modifiers
./example_observer
./example_allocator
./example_growth
./example_algorithms
```

---

## Notes

- `remove_if()` and `erase()` were fixed for correctness during development:
  the original implementation destroyed elements before shifting (undefined
  behavior when non-trivial types are used) and leaked moved-from tail
  elements. Both are now safe for arbitrary destructible `T`.
- `reallocate()` previously leaked the old buffer on every capacity change —
  this has been fixed; the old buffer is always deallocated after a
  successful relocation.
- Copy assignment now reuses existing capacity when possible instead of
  always reallocating via copy-and-swap, trading the strong exception
  guarantee for the basic guarantee on that fast path — matching
  `std::vector`'s actual documented behavior.
- `push_back` and `insert` explicitly guard against self-referential
  arguments (`v.push_back(v[i])`) surviving a reallocation, at the cost of
  an extra pointer-range check per call.
- The observer system is not zero-cost even with no listeners subscribed —
  see [Benchmark Results](#benchmark-results) for measured overhead. A
  compile-time `EnableEvents` template flag (matching the `EnableStats`
  pattern used in the AllocatorPro family) is a planned follow-up.
- `notify()` is called on every mutating operation regardless of whether
  any listeners are subscribed.

---

## License

[MIT](LICENSE) — free to use, modify, and distribute for educational and personal purposes.