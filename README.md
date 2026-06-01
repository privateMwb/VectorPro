# VectorPro

VectorPro is a custom dynamic array container written in modern C++.

It was built to explore:
- manual memory management
- iterators
- move semantics
- exception safety
- container design

This project is primarily educational and is not intended to replace `std::vector`.
The goal is to better understand how dynamic containers and low-level memory
management work internally through a custom implementation.

---

# Features

- Dynamic resizing
- Random access
- Custom iterators
- Reverse iterators
- Copy & move semantics
- `push_back`
- `emplace_back`
- `insert`
- `erase`
- `remove_if`
- `reserve`
- `shrink_to_fit`
- Observer system

---

# Project Structure

```txt
VectorPro/
├── include/
├── benchmarks/
├── examples/
├── test/
├── README.md
└── LICENSE
```

---

# Build

## Example

```bash
g++ -std=c++23 examples/example.cpp -Iinclude -o example
./example
```

## Benchmark

```bash
g++ -std=c++23 benchmarks/benchmark.cpp -Iinclude -o benchmark
./benchmark
```

## Test

```bash
g++ -std=c++23 test/test.cpp -Iinclude -o test
./test
```

---

# License

MIT License
