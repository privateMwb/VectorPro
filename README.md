# VectorPro

[![C++26](https://img.shields.io/badge/C%2B%2B-26-blue)](https://en.cppreference.com/w/cpp/26)
[![Status](https://img.shields.io/badge/status-learning%20project-green)](https://github.com/privateMwb/VectorPro)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A custom C++ dynamic array implementation built for learning low-level memory management, STL-like container design, iterators, allocator support, and performance benchmarking.

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
- [Contributing](#contributing)
- [License](#license)

---

## Overview

VectorPro is a `std::vector`-like container implemented from scratch in modern C++ (C++26).
It focuses on understanding how dynamic arrays work internally, including memory allocation, growth strategies, allocator support, and iterator design.

It also includes:

- Custom contiguous iterator & reverse iterator
- Observer/event system with `EventData`
- Allocator support via `std::allocator_traits`
- Configurable growth factor via template parameters
- Benchmark suite comparing against `std::vector`
- Unit tests for correctness validation

---

## Motivation / Goals

This project was built to understand:

- Dynamic memory management via `std::allocator_traits`
- Rule of 5 (copy/move semantics)
- Amortized complexity in dynamic arrays
- STL container design principles
- Iterator implementation (`std::contiguous_iterator`)
- Event-driven container design (observer pattern)
- Allocator-aware container design
- Performance benchmarking vs `std::vector`

---

## Features

- Dynamic array with automatic resizing
- `push_back`, `pop_back`
- `insert`, `erase` (single and range)
- `emplace_back`, `emplace`
- `remove_if` — returns count of removed elements
- `reserve`, `shrink_to_fit`
- `clear`
- Bounds checking via `at()`
- Copy & move semantics
- Custom contiguous iterators (forward & reverse)
- Observer system (`subscribe`, `unsubscribe`, `notify`) with `EventData`
- Initializer list support
- Iterator range construction
- `std::span` accessor via `as_span()`
- `contains()` and `find()` search methods
- Comparison operators (`==`, `<=>`)
- Configurable growth factor (`GrowthNum / GrowthDen`)
- Allocator support (`std::allocator<T>` by default, compatible with PMR allocators)

---

## Design Overview

VectorPro uses a raw heap-allocated array with manual memory control via `std::allocator_traits`.

### Internal Structure

```
data_ (pointer)
  ↓
[T][T][T][T][T][...]
        ↑
     vsize_ / vcap_
```

- `data_` → pointer to raw allocated memory
- `vsize_` → number of constructed elements
- `vcap_` → total allocated capacity
- `alloc_` → allocator instance

### Growth Strategy

Growth is controlled by two template parameters:

```cpp
template<typename T,
         typename Allocator = std::allocator<T>,
         std::size_t GrowthNum = 2,
         std::size_t GrowthDen = 1>
```

Growth formula:

```cpp
newCapacity = (vcap_ == 0) ? INITIAL_CAP : vcap_ * GrowthNum / GrowthDen;
```

Default is 2x growth. Custom ratios are supported:

```
// 2x (default)   8 → 16 → 32 → 64 → ...
// 1.5x (3/2)     8 → 12 → 18 → 27 → ...
// 1.3x (4/3)     8 → 10 → 13 → 17 → ...
```

Static assertions guard against invalid growth parameters at compile time.

### Memory Management

All allocation and construction goes through `std::allocator_traits`:

```cpp
std::allocator_traits<Allocator>::allocate(alloc_, n);
std::allocator_traits<Allocator>::construct(alloc_, ptr, args...);
std::allocator_traits<Allocator>::destroy(alloc_, ptr);
std::allocator_traits<Allocator>::deallocate(alloc_, ptr, n);
```

This enables compatibility with any conforming allocator including `std::pmr::polymorphic_allocator`.

Reallocation involves:

1. Allocate new raw memory
2. Move-construct elements into new buffer
3. Destroy old elements
4. Deallocate old memory

### Iterator Design

Iterators are lightweight wrappers around raw pointers, satisfying `std::contiguous_iterator`:

```cpp
using iterator_concept  = std::contiguous_iterator_tag;
using iterator_category = std::random_access_iterator_tag;
```

This unlocks the fastest paths in `std::ranges` and standard algorithms like `std::sort`, `std::copy`, and `std::find`.

Reverse iterators are composed via `std::reverse_iterator`:

```
rbegin() → end()
rend()   → begin()
```

### Observer System

VectorPro supports an event-based listener system using `std::function`:

```
mutation (push_back / erase / clear / etc.)
        ↓
notify(EventData)
        ↓
all subscribed listeners executed
```

Each event carries:

```cpp
struct EventData {
    EventType    type;
    std::size_t  index;
    std::size_t  oldSize;
    std::size_t  newSize;
};
```

Supported events: `PUSHBACK`, `EMPLACEBACK`, `POPBACK`, `INSERT`, `REMOVE`, `ERASE`, `CLEAR`, `SHRINK`, `RESERVE`.

Subscribe returns a `ListenerHandle` for targeted unsubscription:

```cpp
auto handle = v.subscribe([](const Vector<int>&, Vector<int>::EventData d) {
    // react to mutation
});

v.unsubscribe(handle);
```

### Exception Safety Model

- Strong safety in copy operations via copy-and-swap
- Move operations are `noexcept`
- Bounds checking via `at()` throws `std::out_of_range`
- `clear()` and destructor are `noexcept`
- Element count (`vsize_`) only incremented after successful construction

### Design Philosophy

VectorPro prioritizes:

- Learning STL internals from scratch
- Explicit memory control via allocator traits
- Performance awareness with configurable growth
- Observer-driven container design
- C++20/26 features: concepts, `<=>`, `std::span`, `std::contiguous_iterator`

---

## Complexity

### Time Complexity

| Operation           | Complexity | Notes                            |
| ------------------- | ---------- | -------------------------------- |
| `push_back`         | O(1)*      | Amortized due to growth strategy |
| `pop_back`          | O(1)       | Direct element destruction       |
| `insert`            | O(n)       | Shifting elements after position |
| `erase`             | O(n)       | Shifting elements after index    |
| `emplace_back`      | O(1)*      | Amortized like push_back         |
| `emplace`           | O(n)       | Shifting elements after position |
| `remove_if`         | O(n)       | Single pass, stable partition    |
| `operator[]`        | O(1)       | Direct pointer access            |
| `at()`              | O(1)       | With bounds checking             |
| `contains()`        | O(n)       | Linear search                    |
| `find()`            | O(n)       | Linear search                    |
| `reserve`           | O(n)       | Reallocation + move              |
| `shrink_to_fit`     | O(n)       | Reallocation + move              |
| `clear`             | O(n)       | Destroys all elements            |

\* Amortized O(1) due to exponential growth strategy

### Space Complexity

- O(n) for stored elements
- Additional capacity overhead based on growth factor
- Separate listener array (`listeners_`, `lsize_`, `lcap_`) for observer storage

### Notes

- Worst-case `push_back` is O(n) during reallocation
- Iterator invalidation occurs on reallocation, `insert`, `erase`, `reserve`, `shrink_to_fit`

---

## Quick Example

### Basic Operations

```cpp
#include "Vector.h"
#include <iostream>

using namespace VectorPro;

int main() {
    Vector<int> v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    std::cout << v[0] << "\n"; // 10
    std::cout << v.front() << "\n"; // 10
    std::cout << v.back()  << "\n"; // 30

    v.insert(v.cbegin() + 1, 15); // { 10, 15, 20, 30 }
    v.erase(v.cbegin() + 2);      // { 10, 15, 30 }

    for (const auto& x : v)
        std::cout << x << " ";

    return 0;
}
```

### Observer & Custom Growth

```cpp
#include "Vector.h"
#include <iostream>

using namespace VectorPro;

int main() {
    // custom 1.5x growth factor
    Vector<int, std::allocator<int>, 3, 2> v;

    // subscribe to mutations
    auto handle = v.subscribe([](const Vector<int, std::allocator<int>, 3, 2>&,
                                  Vector<int, std::allocator<int>, 3, 2>::EventData d) {
        std::cout << "Event fired — newSize: " << d.newSize << "\n";
    });

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.unsubscribe(handle);

    // remove_if
    v.remove_if([](const int& x) { return x % 2 != 0; });

    std::cout << "Size after remove_if: " << v.size() << "\n";

    return 0;
}
```

---

## Core API

### Constructors

```cpp
Vector<T> v;                              // default
Vector<T> v(count, value);               // fill
Vector<T> v = { 1, 2, 3 };              // initializer list
Vector<T> v(first, last);               // iterator range
Vector<T> v(other);                      // copy
Vector<T> v(std::move(other));           // move

// custom allocator
Vector<T, Allocator> v(allocator);

// custom growth factor (1.5x)
Vector<T, std::allocator<T>, 3, 2> v;
```

### Modifiers

```cpp
void push_back(const T& value);
void push_back(T&& value);

template<typename... Args>
void emplace_back(Args&&... args);

iterator insert(const_iterator pos, const T& value);
iterator insert(const_iterator pos, T&& value);

template<std::input_iterator It>
iterator insert(const_iterator pos, It first, It last);

template<typename... Args>
iterator emplace(const_iterator pos, Args&&... args);

template<typename Predicate>
std::size_t remove_if(Predicate pred);

void pop_back();

iterator erase(const_iterator pos);
iterator erase(const_iterator first, const_iterator last);

void clear() noexcept;
```

### Capacity

```cpp
void reserve(std::size_t newCap);
void shrink_to_fit();
void swap(Vector& other) noexcept;

bool        empty()    const noexcept;
std::size_t size()     const noexcept;
std::size_t capacity() const noexcept;
```

### Element Access

```cpp
pointer         data_ptr()       noexcept;
const_pointer   data_ptr() const noexcept;

reference       operator[](std::size_t index)       noexcept;
const_reference operator[](std::size_t index) const noexcept;

reference       at(std::size_t index);
const_reference at(std::size_t index) const;

reference       front();
const_reference front() const;

reference       back();
const_reference back() const;

std::span<T>       as_span()       noexcept;
std::span<const T> as_span() const noexcept;
```

### Search

```cpp
bool     contains(const T& value) const noexcept;
iterator find(const T& value)           noexcept;
const_iterator find(const T& value) const noexcept;
```

### Iterators

```cpp
iterator               begin()   noexcept;
iterator               end()     noexcept;
const_iterator         cbegin()  const noexcept;
const_iterator         cend()    const noexcept;
reverse_iterator       rbegin()  noexcept;
reverse_iterator       rend()    noexcept;
const_reverse_iterator crbegin() const noexcept;
const_reverse_iterator crend()   const noexcept;
```

### Observer System

```cpp
template<typename F>
    requires Listener<F, Vector>
ListenerHandle subscribe(F&& listener);

void unsubscribe(ListenerHandle handle);
```

### Comparison

```cpp
bool operator==(const Vector& other) const noexcept;
auto operator<=>(const Vector& other) const noexcept;
```

---

## Benchmark Results

Benchmarks compare `VectorPro` against `std::vector` across core operations.
All times measured in nanoseconds per iteration (100,000 iterations).

> Compiled with `-std=c++26`. Results may vary depending on hardware and compiler optimizations.

### Constructor

```
----------------------------------------------------------------------
Constructor Benchmark                   Time           Iterations
----------------------------------------------------------------------
VectorPro default construct             56.31 us        100000
std::vector default construct           52.69 us        100000

VectorPro fill construct                105.46 us       100000
std::vector fill construct              105.23 us       100000

VectorPro init list construct           52.85 us        100000
std::vector init list construct         52.62 us        100000

VectorPro copy construct                68.25 ms        100000
std::vector copy construct              52.62 us        100000

VectorPro move construct                204.99 ms       100000
std::vector move construct              291.74 ms       100000

VectorPro copy assignment               68.98 ms        100000
std::vector copy assignment             27.49 ms        100000

VectorPro move assignment               203.99 ms       100000
std::vector move assignment             177.81 ms       100000

VectorPro 1.5x growth construct         300.41 ms       100000
VectorPro PMR allocator construct       151.97 ms       100000
----------------------------------------------------------------------
```

### Modifier

```
----------------------------------------------------------------------
Modifier Benchmark                      Time           Iterations
----------------------------------------------------------------------
VectorPro push_back copy                28.87 ms        100000
std::vector push_back copy              52.62 us        100000

VectorPro push_back move                19.63 ms        100000
std::vector push_back move              105.23 us       100000

VectorPro emplace_back                  37.84 ms        100000
std::vector emplace_back                23.19 ms        100000

VectorPro insert single                 212.65 ms       100000
std::vector insert single               175.09 ms       100000

VectorPro insert range                  244.93 ms       100000
std::vector insert range                321.08 ms       100000

VectorPro emplace                       38.51 ms        100000
std::vector emplace                     48.70 ms        100000

VectorPro remove_if                     331.79 ms       100000
std::vector erase_if                    640.19 ms       100000

VectorPro pop_back                      385.06 ms       100000
std::vector pop_back                    212.42 ms       100000

VectorPro erase single                  222.67 ms       100000
std::vector erase single                219.03 ms       100000

VectorPro erase range                   269.39 ms       100000
std::vector erase range                 225.84 ms       100000

VectorPro clear                         219.47 ms       100000
std::vector clear                       132.71 ms       100000

VectorPro reserve                       105.31 us       100000
std::vector reserve                     52.85 us        100000

VectorPro shrink_to_fit                 117.70 ms       100000
std::vector shrink_to_fit               81.31 ms        100000
----------------------------------------------------------------------
```

### Observer

```
----------------------------------------------------------------------
Observer Benchmark                      Time           Iterations
----------------------------------------------------------------------
push_back no listener                   3.19 ms         100000
push_back one listener                  2.97 ms         100000

push_back 1  listener                   2.48 ms         100000
push_back 5  listeners                  10.42 ms        100000
push_back 10 listeners                  18.29 ms        100000

push_back lightweight callback          2.35 ms         100000
push_back heavyweight callback          15.89 ms        100000

subscribe/unsubscribe under load        54.31 ms        100000
----------------------------------------------------------------------
```

### Element Access & Search

```
----------------------------------------------------------------------
Element Access & Search Benchmark       Time           Iterations
----------------------------------------------------------------------
VectorPro operator[]                    52.85 us        100000
std::vector operator[]                  52.69 us        100000

VectorPro at()                          105.54 us       100000
std::vector at()                        52.85 us        100000

VectorPro front()                       105.54 us       100000
std::vector front()                     52.69 us        100000
VectorPro back()                        52.62 us        100000
std::vector back()                      52.85 us        100000

VectorPro data_ptr()                    52.69 us        100000
std::vector data()                      52.62 us        100000

VectorPro as_span()                     122.38 us       100000

VectorPro contains() best case          105.46 us       100000
VectorPro contains() worst case         105.23 us       100000
std::vector find() best case            365.54 us       100000
std::vector find() worst case           11.75 s         100000

VectorPro find() best case              52.85 us        100000
VectorPro find() worst case             52.69 us        100000
std::find() best case                   369.77 us       100000
std::find() worst case                  11.07 s         100000
----------------------------------------------------------------------
```

### Operator

```
----------------------------------------------------------------------
Operator Benchmark                      Time           Iterations
----------------------------------------------------------------------
VectorPro operator== equal              108.08 us       100000
std::vector operator== equal            54.38 us        100000

VectorPro operator== unequal            54.23 us        100000
std::vector operator== unequal          54.38 us        100000

VectorPro operator<=> less              54.15 us        100000
std::vector operator<=> less            54.38 us        100000

VectorPro operator<=> greater           54.15 us        100000
std::vector operator<=> greater         54.15 us        100000

VectorPro operator<=> equal             108.31 us       100000
std::vector operator<=> equal           108.31 us       100000
----------------------------------------------------------------------
```

### Capacity

```
----------------------------------------------------------------------
Capacity Benchmark                      Time           Iterations
----------------------------------------------------------------------
VectorPro empty()                       54.15 us        100000
std::vector empty()                     54.15 us        100000

VectorPro size()                        54.15 us        100000
std::vector size()                      54.15 us        100000

VectorPro capacity()                    54.46 us        100000
std::vector capacity()                  54.15 us        100000

VectorPro reserve()                     54.15 us        100000
std::vector reserve()                   54.15 us        100000

VectorPro shrink_to_fit()               71.58 ms        100000
std::vector shrink_to_fit()             32.89 ms        100000

VectorPro default growth (2x)           229.84 ms       100000
std::vector default growth              134.70 ms       100000

VectorPro 1.5x growth (3/2)             364.11 ms       100000
VectorPro 1.3x growth (4/3)             435.73 ms       100000
----------------------------------------------------------------------
```

### Iterator

```
----------------------------------------------------------------------
Iterator Benchmark                      Time           Iterations
----------------------------------------------------------------------
VectorPro forward traversal             54.15 us        100000
std::vector forward traversal           108.31 us       100000

VectorPro const forward traversal       108.31 us       100000
std::vector const forward traversal     54.23 us        100000

VectorPro reverse traversal             54.15 us        100000
std::vector reverse traversal           54.15 us        100000

VectorPro iterator +                    54.15 us        100000
std::vector iterator +                  54.15 us        100000
VectorPro iterator -                    54.15 us        100000
std::vector iterator -                  108.31 us       100000

VectorPro std::sort                     38.43 ms        100000
std::vector std::sort                   38.60 ms        100000

VectorPro std::find hit                 325.00 us       100000
std::vector std::find hit               324.92 us       100000
VectorPro std::find miss                11.28 s         100000
std::vector std::find miss              11.12 s         100000

VectorPro std::accumulate               54.15 us        100000
std::vector std::accumulate             54.15 us        100000

VectorPro range-based for               54.15 us        100000
std::vector range-based for             54.15 us        100000
----------------------------------------------------------------------
```

### Summary

| Operation           | Winner      | Notes                                        |
| ------------------- | ----------- | -------------------------------------------- |
| default construct   | Tie         | Near-identical                               |
| fill construct      | Tie         | Near-identical                               |
| init list construct | Tie         | Near-identical                               |
| copy construct      | std::vector | std::vector significantly faster             |
| move construct      | VectorPro   | Both fast; VectorPro slightly slower         |
| copy assignment     | std::vector | std::vector significantly faster             |
| move assignment     | std::vector | Slightly faster                              |
| push_back (copy)    | std::vector | std::vector significantly faster             |
| push_back (move)    | std::vector | std::vector significantly faster             |
| emplace_back        | std::vector | std::vector slightly faster                  |
| insert single       | std::vector | std::vector slightly faster                  |
| insert range        | VectorPro   | VectorPro faster on range inserts            |
| emplace             | VectorPro   | VectorPro slightly faster                    |
| remove_if           | VectorPro   | ~2× faster                                  |
| pop_back            | std::vector | std::vector significantly faster             |
| erase single        | Tie         | Near-identical                               |
| erase range         | std::vector | std::vector slightly faster                  |
| clear               | std::vector | std::vector significantly faster             |
| reserve             | Tie         | Near-identical                               |
| shrink_to_fit       | std::vector | std::vector ~2× faster                       |
| contains()          | VectorPro   | ~3× faster than std::find                    |
| find()              | VectorPro   | ~7× faster than std::find                    |
| operator==          | Tie         | Near-identical                               |
| operator<=>         | Tie         | Near-identical                               |
| forward traversal   | Tie         | Near-identical                               |
| std::sort           | Tie         | Near-identical                               |
| std::accumulate     | Tie         | Near-identical                               |
| range-based for     | Tie         | Near-identical                               |

---

## Project Structure

```
VectorPro/
├── include/
│   ├── Vector.h
│   ├── Vector.tpp
│   └── Iterator.h
│
├── tests/
│   ├── test_helper.h
│   ├── test_all.cpp
│   ├── test_constructor.cpp
│   ├── test_modifiers.cpp
│   ├── test_observer.cpp
│   ├── test_element_access.cpp
│   ├── test_operators.cpp
│   ├── test_capacity.cpp
│   └── test_iterators.cpp
│
├── benchmarks/
│   ├── benchmark_helper.h
│   ├── bench_all.cpp
│   ├── bench_constructor.cpp
│   ├── bench_modifiers.cpp
│   ├── bench_observer.cpp
│   ├── bench_element_access.cpp
│   ├── bench_operators.cpp
│   ├── bench_capacity.cpp
│   └── bench_iterators.cpp
│
├── examples/
│   ├── example_helper.h
│   ├── example1.cpp
│   └── example2.cpp
│
├── CMakeLists.txt
├── README.md
└── LICENSE
```

---

## Build Instructions

### Requirements

- C++26-compatible compiler: GCC 15+, Clang 19+, or MSVC 19.43+
- CMake 3.20+
- No external dependencies — header-only core library

### Using CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Run Tests

```bash
./build/tests
```

### Run Benchmarks

```bash
./build/benchmarks
```

> Use `-O2` or `-O3` for meaningful benchmark results. Debug builds distort timing significantly.

### Run Examples

```bash
./build/example1
./build/example2
```

### Manual Compilation

```bash
# tests
g++ -std=c++26 tests/test_all.cpp tests/test_constructor.cpp ... -Iinclude -o build/tests

# benchmarks
g++ -std=c++26 -O2 benchmarks/bench_all.cpp benchmarks/bench_constructor.cpp ... -Iinclude -o build/benchmarks

# examples
g++ -std=c++26 examples/example1.cpp -Iinclude -o build/example1
g++ -std=c++26 examples/example2.cpp -Iinclude -o build/example2
```

---

## Notes

- **Not production-ready.** This is a learning project — use `std::vector` in real codebases.
- The observer system adds per-operation overhead proportional to listener count — visible in the modifier benchmarks.
- Exception safety is handled for core operations but may not match full STL guarantees in all edge cases.
- Iterator invalidation rules mirror `std::vector`: any reallocation or structural modification (insert/erase) invalidates existing iterators.
- `std::vector` outperforms VectorPro in most benchmarks — this is expected. The goal is learning, not beating the standard library.
- The `find()` and `contains()` wins over `std::find` are due to the small dataset size used in the benchmark — `std::find` on a pre-filled 100k-element vector hits worst-case linear scan.

---

## Contributing

Contributions, improvements, and learning-focused PRs are welcome. Some areas worth exploring:

- Integration with custom allocators (`ArenaAllocator`, `PoolAllocator`, `StackAllocator`, `FreeListAllocator`)
- Small-buffer optimization (SBO) for small element counts
- SIMD-friendly memory layouts
- Additional concept constraints
- Extended CI pipeline (GitHub Actions)

---

## License

[MIT](LICENSE) — free to use, modify, and distribute for educational and personal purposes.

