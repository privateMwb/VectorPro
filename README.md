# VectorPro

<p align="center">
  <img src="https://img.shields.io/github/v/release/privateMwb/VectorPro?style=for-the-badge&logo=github&color=yellow" alt="Version">
  <img src="https://img.shields.io/badge/License-MIT-orange?style=for-the-badge" alt="License - MIT">
  <img src="https://img.shields.io/badge/C%2B%2B-23-blue?style=for-the-badge&logo=c%2B%2B" alt="C++ - 23">
</p>

<p align="center">
  <a href="https://github.com/privateMwb/VectorPro/actions/workflows/build.yml">
    <img src="https://github.com/privateMwb/VectorPro/actions/workflows/build.yml/badge.svg" alt="Build and Test">
  </a>
  <a href="https://github.com/privateMwb/VectorPro/actions/workflows/benchmark.yml">
    <img src="https://github.com/privateMwb/VectorPro/actions/workflows/benchmark.yml/badge.svg" alt="Benchmarks">
  </a>
  <a href="https://github.com/privateMwb/VectorPro/actions/workflows/coverage.yml">
    <img src="https://github.com/privateMwb/VectorPro/actions/workflows/coverage.yml/badge.svg" alt="Coverage">
  </a>
  <a href="https://github.com/privateMwb/VectorPro/actions/workflows/sanitizers.yml">
    <img src="https://github.com/privateMwb/VectorPro/actions/workflows/sanitizers.yml/badge.svg" alt="Sanitizers">
  </a>
  <a href="https://github.com/privateMwb/VectorPro/actions/workflows/clang-tidy.yml">
    <img src="https://github.com/privateMwb/VectorPro/actions/workflows/clang-tidy.yml/badge.svg" alt="Clang Tidy">
  </a>
  <a href="https://github.com/privateMwb/VectorPro/actions/workflows/clang-format.yml">
    <img src="https://github.com/privateMwb/VectorPro/actions/workflows/clang-format.yml/badge.svg" alt="Clang Format">
  </a>
  <a href="https://github.com/privateMwb/VectorPro/actions/workflows/docs.yml">
    <img src="https://github.com/privateMwb/VectorPro/actions/workflows/docs.yml/badge.svg" alt="Documentation">
  </a>
  <a href="https://github.com/privateMwb/VectorPro/actions/workflows/release.yml">
    <img src="https://github.com/privateMwb/VectorPro/actions/workflows/release.yml/badge.svg" alt="Release">
  </a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/GCC-support-B46F1B?style=flat&logo=gnu" alt="GCC - support">
  <img src="https://img.shields.io/badge/Clang-support-045891?style=flat&logo=llvm" alt="Clang - support">
  <img src="https://img.shields.io/badge/MSVC-support-5C2D91?style=flat" alt="MSVC - support">
  <img src="https://img.shields.io/badge/AppleClang-support-000000?style=flat&logo=apple" alt="AppleClang - support">
</p>

A header-only, C++23, `std::vector`-compatible contiguous container with a
configurable growth policy, custom allocator support, and an opt-in
modification-event system.

## 📑 Table of Contents

- [✨ Features](#-features)
- [📋 Requirements](#-requirements)
- [📦 Installation](#-installation)
- [🚀 Quick Start](#-quick-start)
- [🗂️ Project Structure](#️-project-structure)
- [🛠️ Development](#️-development)
- [📊 Benchmarks](#-benchmarks)
- [📖 Documentation](#-documentation)
- [🤝 Contributing](#-contributing)
- [📝 Changelog](#-changelog)
- [📄 License](#-license)

## ✨ Features

- **Familiar `std::vector` API** — `push_back`, `emplace_back`, `insert`,
  `erase`, `remove_if`, forward/reverse iterators, `operator==`/`operator<=>`,
  `swap`, `as_span()`, `contains()`/`find()`, and more.
- **Configurable growth policy** — the capacity growth factor
  (`GrowthNum` / `GrowthDen`) is a template parameter, not hardcoded.
- **Custom allocator support** — works with `std::allocator<T>` or any type
  satisfying the standard allocator interface.
- **Opt-in modification events** — `subscribe()` to be notified on
  `push_back`, `insert`, `erase`, `clear`, `reserve`, `shrink_to_fit`, and
  more, with no cost unless something is actually subscribed.
- **C++23 concepts throughout** — constrained templates
  (`std::destructible`, `std::input_iterator`, `std::predicate`, etc.)
  instead of SFINAE.
- **Strong exception safety** on `insert`, `erase`, and copy-assignment via
  copy-and-swap and rollback on partial failure.
- **`std::contiguous_iterator`-conformant** custom iterator type, usable
  anywhere the standard library expects one.

## 📋 Requirements

- A C++23-conformant compiler (tested: GCC, Clang, MSVC, AppleClang)
- CMake 3.20+

## 📦 Installation

**From source:**

```bash
git clone https://github.com/privateMwb/VectorPro.git
cd VectorPro
cmake -B build \
  -DVECTORPRO_BUILD_TESTS=OFF \
  -DVECTORPRO_BUILD_BENCHMARKS=OFF \
  -DVECTORPRO_BUILD_TOOLS=OFF \
  -DVECTORPRO_BUILD_EXAMPLES=OFF
cmake --install build
```

Then, in your own `CMakeLists.txt`:

```cmake
find_package(VectorPro CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE VectorPro::VectorPro)
```

> vcpkg and Conan packages are built and verified, but not yet published to
> the public registries. This section will be updated once they are.

## 🚀 Quick Start

**Basic usage:**

```cpp
#include <VectorPro/Vector.h>
#include <iostream>

int main() {
    VectorPro::Vector<int> v{1, 2, 3};
    v.push_back(4);

    for (const auto& x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n"; // 1 2 3 4
}
```

**Subscribing to modification events:**

```cpp
#include <VectorPro/Vector.h>
#include <iostream>

int main() {
    VectorPro::Vector<int> v;

    auto handle = v.subscribe([](const auto& vec, auto event) {
        std::cout << "size: " << event.oldSize << " -> " << event.newSize << "\n";
    });

    v.push_back(1); // size: 0 -> 1
    v.push_back(2); // size: 1 -> 2

    v.unsubscribe(handle);
    v.push_back(3); // no output — listener removed
}
```

**Using a custom allocator:**

```cpp
#include <VectorPro/Vector.h>

template <typename T>
struct CountingAllocator : std::allocator<T> {
    static inline std::size_t allocations = 0;

    T* allocate(std::size_t n) {
        ++allocations;
        return std::allocator<T>::allocate(n);
    }
};

int main() {
    VectorPro::Vector<int, CountingAllocator<int>> v;
    v.push_back(42);
    // CountingAllocator<int>::allocations now reflects how many times
    // this vector actually reallocated.
}
```

## 🗂️ Project Structure

```
VectorPro/
├── include/
│   └── VectorPro/
│       ├── Iterator.h
│       ├── Vector.h
│       └── Vector.tpp
│
├── tests/
│   ├── common/
│   ├── suite/
│   ├── test_all.cpp
│   └── CMakeLists.txt
│
├── benchmarks/
│   ├── common/
│   ├── suite/
│   ├── baselines/
│   ├── bench_all.cpp
│   └── CMakeLists.txt
│
├── examples/
│   ├── common/
│   ├── suite/
│   ├── example_all.cpp
│   └── CMakeLists.txt
│
├── tools/
│   ├── regression/
│   └── CMakeLists.txt
│
├── recipes/
│   └── vectorpro/          # Conan recipe
│
├── vcpkg/
│   └── ports/                # vcpkg port
│
├── cmake/
│   └── VectorProConfig.cmake.in
│
├── docs/
│   ├── Doxyfile
│   └── PACKAGING.md
│
├── .gitignore
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## 🛠️ Development

The from-source install above builds the library only. To work on
VectorPro itself — running tests, benchmarks, or the regression tool —
build with everything enabled (the default):

```bash
cmake -B build
cmake --build build
```

**Run the test suite:**

```bash
ctest --test-dir build
```

**Run benchmarks and check for regressions:**

```bash
./build/benchmarks
./build/regression   # current results vs. benchmarks/baselines/v1.0.0.json
```

**Sanitizers, static analysis, and formatting** are covered by the
`Sanitizers`, `Clang Tidy`, and `Clang Format` CI workflows — see
`.github/workflows/` for the exact invocations if you want to reproduce
them locally.

**Build the docs locally:**

```bash
doxygen docs/Doxyfile
```

See `docs/PACKAGING.md` for notes on verifying the vcpkg port and Conan
recipe locally.

## 📊 Benchmarks

Measured against `std::vector`, same build, at 10K / 100K / 1M elements
(`benchmarks/baselines/v1.0.0.json` has the full dataset). Numbers below
are the 100K row unless noted — differences are consistent across scale
for most operations.

**At parity or faster:**

| Operation | VectorPro | std::vector | Difference |
|---|---|---|---|
| `Reserve no-op` | 266.85 μs | 400.62 μs | ~50% faster |
| `Front/back` (10K, 1M) | — | — | ~24-26% faster |
| `Default construct` | 344.92 μs | 419.69 μs | ~22% faster |
| `Insert end` | 161.51 ms | 166.03 ms | ~3% faster |
| `Operator<=> equal` | 3.84 s | 4.08 s | ~6% faster |

**Slower, and consistent across scale — worth fixing:**

| Operation | VectorPro | std::vector | Difference |
|---|---|---|---|
| `Growth reallocation` | 233.41 ms | 127.16 ms | ~46% slower |
| `Push Back` (pre-reserved) | 533.40 ms | 332.74 ms | ~38% slower |
| `Erase-remove If` | 1.11 s | 775.54 ms | ~30% slower |
| `Operator==` (differing sizes) | 133.54 μs | 66.77 μs | ~50% slower |
| `Operator<=> less` | 1.17 ms | 690.23 μs | ~41% slower |
| `Shrink To Fit` | 193.69 ms | 141.89 ms | ~27% slower |
| `Insert front` | 218.50 ms | 181.91 ms | ~17% slower |

<details>
<summary>Why the gap on these specific operations</summary>

Some of this traces to the event-notification hooks and exception-safety
bookkeeping VectorPro performs on every mutation — the cost of the opt-in
observer system and strong exception guarantees. `push_back` itself
carries no measurable per-call cost when nothing is subscribed; the
overhead scales with the number of active listeners (see raw data for
the `Observer` benchmark group). The pre-reserved `Push Back` and
`Growth reallocation` gaps specifically are still under investigation —
their size suggests something beyond notification overhead alone, since
neither involves an active listener.

A few results in the full dataset (`Copy assignment`, `Accumulate` at 1M
scale) show larger deltas than the rest of the suite and are being
re-verified before being treated as reliable — see
`benchmarks/baselines/v1.0.0.json` for the raw numbers if you want to
look yourself.

</details>

## 📖 Documentation

Full API reference (generated with Doxygen, updated on every push to
`main`):

**https://privateMwb.github.io/VectorPro/**

## 🤝 Contributing

Issues and pull requests are welcome. Before submitting a PR:

- Run the test suite (`ctest --test-dir build`)
- Format with `clang-format` (or let the `Clang Format` CI check catch it)
- If you're changing a hot path, run `./build/regression` and mention the
  results in your PR description

## 📝 Changelog

See the [Releases](https://github.com/privateMwb/VectorPro/releases) page
for version history and release notes.

## 📄 License

MIT — see [LICENSE](LICENSE) for details.
