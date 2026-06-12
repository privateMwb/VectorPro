# VectorPro

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)](https://en.cppreference.com/w/cpp/23)
[![Status](https://img.shields.io/badge/status-learning%20project-green)](https://github.com/privateMwb/VectorPro)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)


A custom C++ dynamic array implementation built for learning low-level memory management, STL-like container design, iterators, and performance benchmarking.

---

## Table of Contents

- [Overview](#overview)
- [Motivation / Goals](#motivation--goals)
- [Features](#features)
- [Design Overview](#design-overview)
  - [Internal Structure](#internal-structure)
  - [Growth Strategy](#growth-strategy)
  - [Memory Management](#memory-management)
  - [Iterator Design](#iterator-design)
  - [Observer System](#observer-system)
  - [Exception Safety Model](#exception-safety-model)
  - [Design Philosophy](#design-philosophy)
- [Complexity](#complexity)
  - [Time Complexity](#time-complexity)
  - [Space Complexity](#space-complexity)
- [Quick Example](#quick-example)
  - [Basic Operations](#basic-operations)
  - [Advanced Example](#advanced-example-emplace--remove_if)
- [Core API](#core-api)
  - [Constructors](#constructors)
  - [Modifiers](#modifiers)
  - [Capacity](#capacity)
  - [Element Access](#element-access)
  - [Iterators](#iterators)
  - [Observer System](#observer-system-1)
  - [Utility](#utility)
- [Benchmark Results](#benchmark-results)
  - [Push Back — No Reserve](#push-back--no-reserve)
  - [Push Back — With Reserve](#push-back--with-reserve)
  - [Emplace Back](#emplace-back)
  - [Pop Back](#pop-back)
  - [Insert Front — Worst Case](#insert-front--worst-case)
  - [Insert Middle — Average Case](#insert-middle--average-case)
  - [Erase Front — Worst Case](#erase-front--worst-case)
  - [Erase Middle — Average Case](#erase-middle--average-case)
  - [Remove If](#remove-if)
  - [Iteration](#iteration)
  - [Copy Construction](#copy-construction)
  - [Move Construction](#move-construction)
  - [Summary](#summary)
- [Project Structure](#project-structure)
- [Build Instructions](#build-instructions)
- [Notes](#notes)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

VectorPro is a `std::vector`-like container implemented from scratch in modern C++ (C++23).  
It focuses on understanding how dynamic arrays work internally, including memory allocation, growth strategies, and iterator support.

It also includes:

- Custom iterator & reverse iterator
- Observer/event system (`notify`)
- Benchmark suite comparing against `std::vector`
- Unit tests for correctness validation

---

## Motivation / Goals

This project was built to understand:

- Dynamic memory management (`new`, placement new, `operator delete`)
- Rule of 5 (copy/move semantics)
- Amortized complexity in dynamic arrays
- STL container design principles
- Iterator implementation
- Event-driven container design (observer pattern)
- Performance benchmarking vs `std::vector`

---

## Features

- Dynamic array with automatic resizing
- `push_back`, `pop_back`
- `insert`, `erase`
- `emplace_back`
- `remove_if`
- `reserve`, `shrink_to_fit`
- `clear`
- Bounds checking via `at()`
- Copy & move semantics
- Custom iterators (forward & reverse)
- Observer system (`subscribe`, `notify`)
- Initializer list support

---

## Design Overview

VectorPro uses a raw heap-allocated array with manual memory control.

It is designed to behave similarly to `std::vector`, but implemented from scratch for educational purposes.

### Internal Structure

```
data (T*)
  ↓
[T][T][T][T][T][...]
        ↑
     v_size / v_cap
```

- `data` → pointer to raw heap memory
- `v_size` → number of constructed elements
- `v_cap` → total allocated capacity

### Growth Strategy

When capacity is exceeded:

```cpp
new_capacity = (v_cap == 0)
             ? 8
             : v_cap + (v_cap / 2);
```

This ensures:

- Amortized O(1) `push_back`
- Gradual growth (reduces realloc frequency)
- Balanced memory vs performance tradeoff

```
8 → 12 → 18 → 27 → 40 → 60 → ...
```

### Memory Management

VectorPro manually controls object lifetime using placement new:

```cpp
new (data + i) T(value);
```

And explicit destruction:

```cpp
data[i].~T();
```

This enables:

- Fine-grained control of construction/destruction
- Avoiding default allocator behavior
- Learning low-level memory handling

Reallocation involves:

1. Allocate new raw memory
2. Move or copy elements
3. Destroy old elements
4. Free old memory

### Iterator Design

Iterators are lightweight wrappers around raw pointers:

```
[T* ptr]
```

They support:

- Dereference (`*ptr`)
- Increment (`++ptr`)
- Comparison (`ptr != end`)

Reverse iterator:

```
rbegin() → end()
rend()   → begin()
```

Enables range-based for loops:

```cpp
for (auto& x : v) {
    // iteration
}
```

### Observer System

VectorPro supports an event-based listener system.

```
operation (push/erase/clear/etc)
        ↓
notify(EventType)
        ↓
all subscribed listeners executed
```

Supported events:

- PushBack
- PopBack
- Insert
- Erase
- Clear
- Reserve
- Shrink

This allows external systems to react to container changes.

### Exception Safety Model

- Strong safety in copy and move operations
- Reallocation uses rollback on failure
- Bounds checking via `at()` throws `std::out_of_range`
- Manual cleanup ensures no memory leaks on exceptions

### Design Philosophy

VectorPro prioritizes:

- Learning STL internals
- Explicit memory control
- Performance awareness
- Minimal abstraction over raw arrays
- Understanding how std::vector works under the hood

---

## Complexity

VectorPro is designed to match `std::vector`-like performance characteristics.

### Time Complexity

| Operation           | Complexity | Notes                            |
| ------------------- | ---------- | -------------------------------- |
| push\_back          | O(1)\*     | Amortized due to growth strategy |
| pop\_back           | O(1)       | Direct element destruction       |
| insert              | O(n)       | Shifting elements after position |
| erase               | O(n)       | Shifting elements after index    |
| emplace\_back       | O(1)\*     | Amortized like push\_back        |
| access (operator[]) | O(1)       | Direct pointer access            |
| at()                | O(1)       | With bounds checking             |
| reserve             | O(n)       | Reallocation + move              |
| shrink\_to\_fit     | O(n)       | Reallocation + move              |
| clear               | O(n)       | Destroys all elements            |

\* Amortized O(1) due to exponential growth strategy

### Space Complexity

- O(n) for stored elements
- Additional capacity overhead due to growth strategy
- Minimal overhead beyond raw array + listener array

### Notes

- Worst-case `push_back` is O(n) during reallocation
- Iterator invalidation occurs on:
  - reallocation
  - insert
  - erase
  - reserve
  - shrink_to_fit

---

## Quick Example

A minimal example showing basic usage of `VectorPro`.

### Basic Operations

```cpp
#include "VectorPro.h"
#include <iostream>

int main() {
    VectorPro<int> v;

    // push elements
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    // access elements
    std::cout << v[0] << "\n"; // 10
    std::cout << v[1] << "\n"; // 20

    // insert in middle
    v.insert(1, 15); // {10, 15, 20, 30}

    // erase element
    v.erase(2); // {10, 15, 30}

    // iterate
    for (auto& x : v) {
        std::cout << x << " ";
    }

    return 0;
}
```

### Advanced Example (emplace + remove_if)

```cpp
#include "VectorPro.h"
#include <string>

struct Person {
    std::string name;
    int age;

    Person(std::string n, int a)
        : name(std::move(n)), age(a) {}
};

int main() {
    VectorPro<Person> people;

    people.emplace_back("Alice", 20);
    people.emplace_back("Bob", 25);
    people.emplace_back("Charlie", 17);

    // remove minors
    people.remove_if([](const Person& p) {
        return p.age < 18;
    });

    return 0;
}
```

---

## Core API

### Constructors

```cpp
VectorPro<T> v;                          // default
VectorPro<T> v{1, 2, 3};                 // initializer list
VectorPro<T> v(other);                   // copy constructor
VectorPro<T> v(std::move(other));        // move constructor
```

### Modifiers

```cpp
void push_back(const T& value);
void push_back(T&& value);

template<typename... Args>
void emplace_back(Args&&... args);

void pop_back();

void insert(size_type index, const T& value);
void insert(size_type index, T&& value);

void erase(size_type index);

template<typename Predicate>
void remove_if(Predicate pred);

void clear();
```

### Capacity

```cpp
void reserve(size_type newCap);
void shrink_to_fit();

bool empty() const;
size_type size() const;
size_type capacity() const;
```

### Element Access

```cpp
T& operator[](size_type index);
const T& operator[](size_type index) const;

T& at(size_type index);
const T& at(size_type index) const;

T& front();
T& back();
```

### Iterators

```cpp
iterator begin();
iterator end();

const_iterator begin() const;
const_iterator end() const;

reverse_iterator rbegin();
reverse_iterator rend();
```

### Observer System

```cpp
template<typename Func>
void subscribe(Func listener);

void unsubscribe(size_type index);
```

### Utility

```cpp
static void swap(VectorPro& a, VectorPro& b);
```

---

## Benchmark Results

Benchmarks compare `VectorPro` against `std::vector` across core operations.
All times measured in microseconds (µs).

> Results may vary depending on compiler optimizations and hardware.
> Compiled with `-O2`.

### Push Back — No Reserve

| Elements | VectorPro  | std::vector | Speedup      |
| -------- | ---------- | ----------- | ------------ |
| 500K     | 33,835 µs  | 38,664 µs   | ~1.14×       |
| 1M       | 53,032 µs  | 77,677 µs   | ~1.46×       |
| 2M       | 118,784 µs | 154,839 µs  | ~1.30×       |
| 4M       | 256,010 µs | 310,441 µs  | ~1.21×       |

### Push Back — With Reserve

| Elements | VectorPro  | std::vector | Speedup      |
| -------- | ---------- | ----------- | ------------ |
| 500K     | 15,544 µs  | 36,671 µs   | ~2.36×       |
| 1M       | 30,826 µs  | 72,916 µs   | ~2.37×       |
| 2M       | 61,307 µs  | 146,070 µs  | ~2.38×       |
| 4M       | 122,678 µs | 291,072 µs  | ~2.37×       |

Pre-reserving capacity exposes VectorPro's strongest advantage — roughly **2.4× faster** than `std::vector` when reallocation is eliminated.

### Emplace Back

| Elements | VectorPro  | std::vector | Speedup      |
| -------- | ---------- | ----------- | ------------ |
| 500K     | 14,077 µs  | 31,288 µs   | ~2.22×       |
| 1M       | 30,841 µs  | 65,363 µs   | ~2.12×       |
| 2M       | 61,873 µs  | 130,396 µs  | ~2.11×       |
| 4M       | 123,486 µs | 260,195 µs  | ~2.11×       |

### Pop Back

| Elements | VectorPro  | std::vector | Speedup      |
| -------- | ---------- | ----------- | ------------ |
| 500K     | 10,819 µs  | 16,131 µs   | ~1.49×       |
| 1M       | 21,317 µs  | 32,152 µs   | ~1.51×       |
| 2M       | 42,550 µs  | 64,495 µs   | ~1.52×       |
| 4M       | 85,056 µs  | 129,095 µs  | ~1.52×       |

### Insert Front — Worst Case

| Elements | VectorPro   | std::vector | Speedup      |
| -------- | ----------- | ----------- | ------------ |
| 5K       | 3,690 µs    | 4,820 µs    | ~1.31×       |
| 10K      | 13,894 µs   | 16,300 µs   | ~1.17×       |
| 20K      | 59,506 µs   | 64,240 µs   | ~1.08×       |
| 40K      | 321,890 µs  | 330,114 µs  | ~1.03×       |

### Insert Middle — Average Case

| Elements | VectorPro   | std::vector | Speedup      |
| -------- | ----------- | ----------- | ------------ |
| 5K       | 5,379 µs    | 6,581 µs    | ~1.22×       |
| 10K      | 20,715 µs   | 23,068 µs   | ~1.11×       |
| 20K      | 91,521 µs   | 96,879 µs   | ~1.06×       |
| 40K      | 521,943 µs  | 532,839 µs  | ~1.02×       |

As element count grows, O(n) shifting dominates and the gap narrows — expected behavior.

### Erase Front — Worst Case

| Elements | VectorPro   | std::vector | Speedup      |
| -------- | ----------- | ----------- | ------------ |
| 5K       | 3,575 µs    | 4,652 µs    | ~1.30×       |
| 10K      | 13,678 µs   | 15,895 µs   | ~1.16×       |
| 20K      | 57,344 µs   | 62,408 µs   | ~1.09×       |
| 40K      | 302,379 µs  | 311,615 µs  | ~1.03×       |

### Erase Middle — Average Case

| Elements | VectorPro   | std::vector | Speedup      |
| -------- | ----------- | ----------- | ------------ |
| 5K       | 1,395 µs    | 1,949 µs    | ~1.40×       |
| 10K      | 5,224 µs    | 6,381 µs    | ~1.22×       |
| 20K      | 20,349 µs   | 22,688 µs   | ~1.11×       |
| 40K      | 88,446 µs   | 93,379 µs   | ~1.06×       |

### Remove If (avg of 5 cycles)

| Elements | VectorPro  | std::vector | Speedup      |
| -------- | ---------- | ----------- | ------------ |
| 100K     | 1,247 µs   | 5,015 µs    | ~4.02×       |
| 200K     | 2,494 µs   | 10,010 µs   | ~4.01×       |
| 400K     | 4,966 µs   | 20,022 µs   | ~4.03×       |
| 800K     | 9,968 µs   | 39,986 µs   | ~4.01×       |

`remove_if` is VectorPro's most significant win — approximately **4× faster** than `std::vector`, scaling consistently with element count.

### Iteration

| Elements | VectorPro   | std::vector | Notes              |
| -------- | ----------- | ----------- | ------------------ |
| 1M       | 28,598 µs   | 24,288 µs   | std::vector faster |
| 2M       | 57,517 µs   | 48,609 µs   | std::vector faster |
| 4M       | 114,738 µs  | 97,098 µs   | std::vector faster |
| 8M       | 229,734 µs  | 194,227 µs  | std::vector faster |

`std::vector` is ~18% faster during iteration, likely due to tighter allocator and cache-line alignment optimizations not replicated here.

### Copy Construction

| Elements | VectorPro  | std::vector | Notes            |
| -------- | ---------- | ----------- | ---------------- |
| 500K     | 6,887 µs   | 7,099 µs    | Near-identical   |
| 1M       | 14,823 µs  | 14,933 µs   | Near-identical   |
| 2M       | 29,417 µs  | 28,772 µs   | Near-identical   |
| 4M       | 55,034 µs  | 56,712 µs   | Near-identical   |

Copy construction performance is essentially the same — both are bounded by the cost of the memory copy.

### Move Construction

| Elements | VectorPro | std::vector | Notes                      |
| -------- | --------- | ----------- | -------------------------- |
| 500K     | 3 µs      | 1 µs        | Both near-zero             |
| 1M       | 4 µs      | 1 µs        | Both near-zero             |
| 2M       | 3 µs      | 1 µs        | Both near-zero             |
| 4M       | 4 µs      | 1 µs        | Both near-zero             |

Both implementations achieve near-zero move construction (pointer swap only). The small raw difference is within measurement noise but may reflect minor overhead in VectorPro's observer state transfer.

### Summary

| Operation              | Winner       | Notes                                      |
| ---------------------- | ------------ | ------------------------------------------ |
| push_back (no reserve) | VectorPro    | ~1.2–1.5× faster                          |
| push_back (reserved)   | VectorPro    | ~2.4× faster                              |
| emplace_back           | VectorPro    | ~2.1× faster                              |
| pop_back               | VectorPro    | ~1.5× faster                              |
| insert (front/middle)  | VectorPro    | Slight edge; gap narrows at scale          |
| erase (front/middle)   | VectorPro    | Slight edge; gap narrows at scale          |
| remove_if              | VectorPro    | ~4× faster                                |
| iteration              | std::vector  | ~18% faster                               |
| copy construction      | Tie          | Effectively identical                      |
| move construction      | std::vector  | Both near-zero; negligible difference      |

---

## Project Structure

```
VectorPro/
├── include/
│   ├── VectorPro.h
│   ├── VectorPro.tpp
│   └── Iterator.h
│
├── benchmarks/
│   ├── benchmarks.cpp
│   └── utils/
│       ├── Table.h
│       └── Table.tpp
│
├── tests/
│   └── test.cpp
│
├── examples/
│   └── examples.cpp
│
├── README.md
└── LICENSE
```

---

## Build Instructions

### Requirements

- C++23-compatible compiler: GCC 13+, Clang 17+, or MSVC 19.38+
- No external dependencies — header-only core library

### Compile & Run Tests

```bash
g++ -std=c++23 tests/test.cpp -Iinclude -o build/tests
./build/tests
```

### Compile & Run Benchmarks

```bash
g++ -std=c++23 -O2 benchmarks/benchmarks.cpp -Iinclude -Ibenchmarks/utils -o build/benchmarks
./build/benchmarks
```

> Use `-O2` or `-O3` for meaningful benchmark results. Debug builds distort timing significantly.

### Compile & Run Examples

```bash
g++ -std=c++23 examples/examples.cpp -Iinclude -o build/examples
./build/examples
```

---

## Notes

- **Not production-ready.** This is an educational project — use `std::vector` in real codebases.
- Some `std::vector` optimizations are intentionally omitted: allocator traits, small-buffer optimization (SBO), SIMD-friendly layouts, etc.
- Exception safety is handled for core operations but may not match full STL guarantees in all edge cases.
- The observer system adds a small per-operation overhead — this is visible in the `emplace_back` and `pop_back` benchmarks.
- Iterator invalidation rules mirror `std::vector`: any reallocation or structural modification (insert/erase) invalidates existing iterators.

---

## Contributing

Contributions, improvements, and learning-focused PRs are welcome! Some areas worth exploring:

- Allocator support (`std::allocator_traits`)
- `std::span` compatibility
- Additional iterator categories (random-access conformance)
- CMake build system integration
- CI pipeline (GitHub Actions)

---

## License

[MIT](LICENSE) — free to use, modify, and distribute for educational and personal purposes.
