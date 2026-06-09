# VectorPro

![C++](https://img.shields.io/badge/C%2B%2B-23-blue)
![Status](https://img.shields.io/badge/status-learning-green)

A custom C++ dynamic array implementation built for learning low-level memory management, STL-like container design, iterators, and performance benchmarking.

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

```text
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

```text
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

```text
[T* ptr]
```

They support:

- Dereference (`*ptr`)
- Increment (`++ptr`)
- Comparison (`ptr != end`)

Reverse iterator:

```text
rbegin() → end()
rend() → begin()
```

Enables range-based for loops:

```cpp
for (auto& x : v) {
    // iteration
}
```


### Observer System

VectorPro supports an event-based listener system.

```text
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

| Operation        | Complexity | Notes |
|----------------|------------|------|
| push_back      | O(1)*      | Amortized due to growth strategy |
| pop_back       | O(1)       | Direct element destruction |
| insert         | O(n)       | Shifting elements after position |
| erase          | O(n)       | Shifting elements after index |
| emplace_back   | O(1)*      | Amortized like push_back |
| access (operator[]) | O(1)   | Direct pointer access |
| at()           | O(1)       | With bounds checking |
| reserve        | O(n)       | Reallocation + move |
| shrink_to_fit  | O(n)       | Reallocation + move |
| clear          | O(n)       | Destroys all elements |

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

> Results may vary depending on compiler optimizations and hardware.


### Push Back (Amortized Growth)

| Elements | VectorPro | std::vector |
|----------|----------|--------------|
| 1M       | 24 ms    | 21 ms        |
| 2M       | 55 ms    | 48 ms        |
| 4M       | 100 ms   | 86 ms        |
| 8M       | 215 ms   | 174 ms       |


### Pop Back

| Elements | VectorPro | std::vector |
|----------|----------|--------------|
| 100K     | 0 ms     | 0 ms         |
| 200K     | 0 ms     | 0 ms         |
| 400K     | 1 ms     | 0 ms         |
| 800K     | 2 ms     | 0 ms         |


### Insert (Middle Shifting)

| Elements | VectorPro | std::vector |
|----------|----------|--------------|
| 10K      | 29 ms    | 29 ms        |
| 20K      | 407 ms   | 407 ms       |
| 40K      | 2180 ms  | 2185 ms      |
| 80K      | 11240 ms | 10922 ms     |


### Erase (Front Shifting)

| Elements | VectorPro | std::vector |
|----------|----------|--------------|
| 10K      | 14 ms    | 14 ms        |
| 20K      | 63 ms    | 63 ms        |
| 40K      | 334 ms   | 334 ms       |
| 80K      | 1551 ms  | 1531 ms      |


### Emplace Back

| Elements | VectorPro | std::vector |
|----------|----------|--------------|
| 1M       | 24 ms    | 9 ms         |
| 2M       | 55 ms    | 26 ms        |
| 4M       | 106 ms   | 39 ms        |
| 8M       | 220 ms   | 79 ms        |


### Remove If

| Elements | VectorPro | std::vector |
|----------|----------|--------------|
| 100K     | 0 ms     | 0 ms         |
| 200K     | 0 ms     | 0 ms         |
| 400K     | 1 ms     | 1 ms         |
| 800K     | 2 ms     | 2 ms         |


### Summary

- `push_back` shows amortized O(1) behavior
- `insert` and `erase` show expected O(n) shifting cost
- `emplace_back` is faster in `std::vector`
- `remove_if` behaves similarly in both implementations
- differences come from allocator + implementation overhead

---

## Project Structure

```text
VectorPro/
├── include/
│   ├── VectorPro.h
│   ├── VectorPro.tpp
│   ├── Iterator.h
│   └── ReverseIterator.h
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
- C++23 compatible compiler (GCC / Clang / MSVC)
- CMake (optional, if you later switch to build system)
- Git (for cloning repository)


### Compile Tests

```bash
g++ -std=c++23 tests/vectorpro_tests.cpp -Iinclude -o build/tests
./build/tests
```


### Compile Benchmarks

```bash
g++ -std=c++23 benchmarks/benchmarks.cpp -Iinclude -Ibenchmarks/utils -O2 -o build/benchmarks
./build/benchmarks
```


### Compile Examples

```bash
g++ -std=c++23 examples/main.cpp -Iinclude -o build/examples
./build/examples
```


### Notes
- Use `-O2` or `-O3` for benchmark accuracy
- Debug builds (`-g`) may distort performance results
```

---

## Notes

- This project is purely educational and not intended as a production-ready STL replacement
- Some optimizations used in `std::vector` are not fully replicated (allocator traits, small buffer optimizations, etc.)
- Performance results may vary depending on compiler, platform, and optimization flags
- The implementation prioritizes learning clarity over micro-optimizations
- Exception safety is handled in core operations but may not match full STL guarantees in all edge cases

---

## License

MIT License

You are free to use, modify, and distribute this project for educational and personal purposes.
