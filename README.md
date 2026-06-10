# VectorPro

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)](https://en.cppreference.com/w/cpp/23)
[![Status](https://img.shields.io/badge/status-learning%20project-green)](https://github.com/privateMwb/VectorPro)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A custom `std::vector`-like dynamic array implemented from scratch in **C++23**, built for learning low-level memory management, STL container design, iterators, and performance benchmarking.

---

## Table of Contents

- [Overview](#overview)
- [Motivation](#motivation)
- [Features](#features)
- [Project Structure](#project-structure)
- [Design Overview](#design-overview)
  - [Internal Structure](#internal-structure)
  - [Growth Strategy](#growth-strategy)
  - [Memory Management](#memory-management)
  - [Iterator Design](#iterator-design)
  - [Observer System](#observer-system)
  - [Exception Safety](#exception-safety)
- [Complexity](#complexity)
- [Quick Start](#quick-start)
- [API Reference](#api-reference)
- [Benchmark Results](#benchmark-results)
- [Build Instructions](#build-instructions)
- [Notes](#notes)
- [License](#license)

---

## Overview

VectorPro is a feature-complete, `std::vector`-compatible container built from the ground up — no STL containers, no default allocator. It covers the full breadth of what makes `std::vector` work: amortized growth, placement-new lifetime management, copy/move semantics, iterator support, and more.

It also goes a step further by adding an **observer/event system** that lets external code react to container mutations in real time.

---

## Motivation

This project was built to deeply understand:

- Dynamic memory management (`new`, placement `new`, `operator delete`)
- The Rule of 5 (copy/move constructors and assignment operators)
- Amortized complexity in dynamic arrays
- STL container design principles
- Iterator and reverse iterator implementation
- Event-driven container design (observer pattern)
- Real performance tradeoffs vs `std::vector`

---

## Features

- Dynamic array with automatic resizing
- Full set of modifiers: `push_back`, `pop_back`, `insert`, `erase`, `emplace_back`, `remove_if`, `clear`
- Capacity management: `reserve`, `shrink_to_fit`
- Bounds-checked access via `at()`
- Copy & move semantics (Rule of 5)
- Custom forward and reverse iterators
- Range-based for loop support
- Initializer list construction
- Observer/event system (`subscribe`, `notify`, `unsubscribe`)

---

## Project Structure

```
VectorPro/
├── include/
│   ├── VectorPro.h          # Main template class declaration
│   ├── VectorPro.tpp        # Template method definitions
│   ├── Iterator.h           # Forward iterator
│   └── ReverseIterator.h    # Reverse iterator
│
├── benchmarks/
│   ├── benchmarks.cpp       # Benchmark suite vs std::vector
│   └── utils/
│       ├── Table.h          # Benchmark result formatting
│       └── Table.tpp
│
├── tests/
│   └── test.cpp             # Unit tests for correctness validation
│
├── examples/
│   └── examples.cpp         # Usage examples
│
├── README.md
└── LICENSE
```

---

## Design Overview

### Internal Structure

VectorPro maintains three core members on a raw heap-allocated block:

```
data (T*)
  ↓
[ T ][ T ][ T ][ T ][ T ][ ... ]
                  ↑           ↑
               v_size       v_cap
```

| Member   | Role                                  |
|----------|---------------------------------------|
| `data`   | Pointer to raw heap memory            |
| `v_size` | Number of live, constructed elements  |
| `v_cap`  | Total allocated capacity (slots)      |

---

### Growth Strategy

When capacity is exceeded during `push_back` or `insert`, VectorPro grows according to:

```cpp
new_capacity = (v_cap == 0) ? 8 : v_cap + (v_cap / 2);
```

This 1.5× growth factor (vs `std::vector`'s typical 2×) produces:

```
8 → 12 → 18 → 27 → 40 → 60 → 90 → ...
```

**Tradeoffs:**
- Fewer reallocations than linear growth → amortized O(1) `push_back`
- Smaller memory headroom vs 2× growth → better memory efficiency
- Slightly more reallocations than 2× growth → marginal performance cost

---

### Memory Management

VectorPro manually controls object lifetime using placement `new` for construction:

```cpp
new (data + i) T(value);   // Construct in-place
```

And explicit destructor calls for destruction:

```cpp
data[i].~T();              // Destroy without freeing memory
```

This separates allocation from construction — the same technique used internally by `std::vector`. Reallocation follows four steps:

1. Allocate new raw memory block
2. Move (or copy) existing elements into it
3. Destroy elements in the old block
4. Free the old memory

---

### Iterator Design

Iterators wrap a raw pointer with a minimal interface:

```cpp
struct Iterator {
    T* ptr;

    T& operator*()  { return *ptr; }
    Iterator& operator++() { ++ptr; return *this; }
    bool operator!=(const Iterator& o) const { return ptr != o.ptr; }
};
```

The reverse iterator maps:

```
rbegin() → points to last element  (end() - 1)
rend()   → points before first     (begin() - 1)
```

Both iterators support range-based for loops:

```cpp
for (auto& x : v) { /* ... */ }
```

> **Note:** Iterators are invalidated by any operation that causes reallocation (`push_back` when full, `reserve`, `shrink_to_fit`) or shifts elements (`insert`, `erase`).

---

### Observer System

VectorPro supports an event-listener system. Subscribers receive an `EventType` enum value whenever the container mutates:

```
Container operation
       ↓
  notify(EventType)
       ↓
All subscribed listeners are called
```

**Supported events:**

| Event       | Triggered by      |
|-------------|-------------------|
| `PushBack`  | `push_back`       |
| `PopBack`   | `pop_back`        |
| `Insert`    | `insert`          |
| `Erase`     | `erase`           |
| `Clear`     | `clear`           |
| `Reserve`   | `reserve`         |
| `Shrink`    | `shrink_to_fit`   |

**Example:**

```cpp
VectorPro<int> v;

v.subscribe([](EventType e) {
    if (e == EventType::PushBack)
        std::cout << "Element added!\n";
});

v.push_back(42); // → "Element added!"
```

---

### Exception Safety

| Scenario                         | Guarantee         |
|----------------------------------|-------------------|
| Copy/move construction           | Strong            |
| Reallocation failure             | Rollback (strong) |
| `at()` out-of-bounds             | Throws `std::out_of_range` |
| Manual cleanup on exception      | No memory leaks   |

---

## Complexity

### Time Complexity

| Operation       | Average    | Worst Case | Notes                              |
|-----------------|------------|------------|------------------------------------|
| `push_back`     | O(1)       | O(n)       | Amortized; O(n) on reallocation    |
| `emplace_back`  | O(1)       | O(n)       | Same as `push_back`                |
| `pop_back`      | O(1)       | O(1)       | Destructs last element             |
| `insert`        | O(n)       | O(n)       | Shifts elements rightward          |
| `erase`         | O(n)       | O(n)       | Shifts elements leftward           |
| `operator[]`    | O(1)       | O(1)       | Direct pointer offset              |
| `at()`          | O(1)       | O(1)       | Same + bounds check                |
| `reserve`       | O(n)       | O(n)       | Realloc + move                     |
| `shrink_to_fit` | O(n)       | O(n)       | Realloc + move                     |
| `clear`         | O(n)       | O(n)       | Destructs all elements             |
| `remove_if`     | O(n)       | O(n)       | Single-pass predicate filter       |

### Space Complexity

- **O(n)** for stored elements
- Additional overhead from growth strategy (up to ~50% excess capacity)
- Listener array adds minimal overhead proportional to subscriber count

---

## Quick Start

### Basic Operations

```cpp
#include "VectorPro.h"
#include <iostream>

int main() {
    VectorPro<int> v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    std::cout << v[0] << "\n";   // 10
    std::cout << v.size() << "\n"; // 3

    v.insert(1, 15);  // {10, 15, 20, 30}
    v.erase(2);       // {10, 15, 30}

    for (auto& x : v)
        std::cout << x << " ";  // 10 15 30
}
```

### Emplace + remove_if

```cpp
#include "VectorPro.h"
#include <string>

struct Person {
    std::string name;
    int age;
    Person(std::string n, int a) : name(std::move(n)), age(a) {}
};

int main() {
    VectorPro<Person> people;

    people.emplace_back("Alice",   20);
    people.emplace_back("Bob",     25);
    people.emplace_back("Charlie", 17);

    // Remove anyone under 18
    people.remove_if([](const Person& p) { return p.age < 18; });
    // Remaining: Alice, Bob
}
```

### Initializer List + Observer

```cpp
VectorPro<int> v{1, 2, 3, 4, 5};

v.subscribe([](EventType e) {
    std::cout << "Event: " << static_cast<int>(e) << "\n";
});

v.push_back(6);  // triggers PushBack event
v.pop_back();    // triggers PopBack event
```

---

## API Reference

### Constructors

```cpp
VectorPro<T> v;                    // Default (empty)
VectorPro<T> v{1, 2, 3};           // Initializer list
VectorPro<T> v(other);             // Copy constructor
VectorPro<T> v(std::move(other));  // Move constructor
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

bool      empty()    const;
size_type size()     const;
size_type capacity() const;
```

### Element Access

```cpp
T&       operator[](size_type index);
const T& operator[](size_type index) const;

T&       at(size_type index);        // Bounds-checked; throws std::out_of_range
const T& at(size_type index) const;

T& front();
T& back();
```

### Iterators

```cpp
iterator       begin();    iterator       end();
const_iterator begin() const; const_iterator end() const;
reverse_iterator rbegin(); reverse_iterator rend();
```

### Observer System

```cpp
template<typename Func>
void subscribe(Func listener);       // Register a listener

void unsubscribe(size_type index);   // Remove listener by index
```

### Utility

```cpp
static void swap(VectorPro& a, VectorPro& b);
```

---

## Benchmark Results

Benchmarks compare `VectorPro` against `std::vector` over representative workloads. Results were collected without `-O2`/`-O3`; **always use optimized builds for real performance comparisons**.

### push_back (1.5× growth)

| Elements | VectorPro | std::vector |
|----------|-----------|-------------|
| 1M       | 24 ms     | 21 ms       |
| 2M       | 55 ms     | 48 ms       |
| 4M       | 100 ms    | 86 ms       |
| 8M       | 215 ms    | 174 ms      |

### pop_back

| Elements | VectorPro | std::vector |
|----------|-----------|-------------|
| 100K     | 0 ms      | 0 ms        |
| 200K     | 0 ms      | 0 ms        |
| 400K     | 1 ms      | 0 ms        |
| 800K     | 2 ms      | 0 ms        |

### insert (middle shifting)

| Elements | VectorPro | std::vector |
|----------|-----------|-------------|
| 10K      | 29 ms     | 29 ms       |
| 20K      | 407 ms    | 407 ms      |
| 40K      | 2180 ms   | 2185 ms     |
| 80K      | 11240 ms  | 10922 ms    |

### erase (front shifting)

| Elements | VectorPro | std::vector |
|----------|-----------|-------------|
| 10K      | 14 ms     | 14 ms       |
| 20K      | 63 ms     | 63 ms       |
| 40K      | 334 ms    | 334 ms      |
| 80K      | 1551 ms   | 1531 ms     |

### emplace_back

| Elements | VectorPro | std::vector |
|----------|-----------|-------------|
| 1M       | 24 ms     | 9 ms        |
| 2M       | 55 ms     | 26 ms       |
| 4M       | 106 ms    | 39 ms       |
| 8M       | 220 ms    | 79 ms       |

### remove_if

| Elements | VectorPro | std::vector |
|----------|-----------|-------------|
| 100K     | 0 ms      | 0 ms        |
| 200K     | 0 ms      | 0 ms        |
| 400K     | 1 ms      | 1 ms        |
| 800K     | 2 ms      | 2 ms        |

### Takeaways

- `push_back`, `insert`, `erase`, and `remove_if` are **on par** with `std::vector`
- `emplace_back` is noticeably slower — likely due to missing allocator-trait optimizations and the overhead of the observer notify path
- `pop_back` shows a minor difference at scale, possibly due to the same observer overhead
- All operations maintain the **expected asymptotic complexity**

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
