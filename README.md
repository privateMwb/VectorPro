# VectorPro

<p align="center">
  <img src="https://img.shields.io/github/v/release/privateMwb/VectorPro?style=for-the-badge&logo=github&color=yellow" alt="Version">
  <img src="https://img.shields.io/badge/License-MIT-orange?style=for-the-badge" alt="License - MIT">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue?style=for-the-badge&logo=c%2B%2B" alt="C++ - 20">
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
  <a href="https://github.com/privateMwb/VectorPro/actions/workflows/packaging.yml">
    <img src="https://github.com/privateMwb/VectorPro/actions/workflows/packaging.yml/badge.svg" alt="Packaging">
  </a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/GCC-support-B46F1B?style=flat&logo=gnu" alt="GCC - support">
  <img src="https://img.shields.io/badge/Clang-support-045891?style=flat&logo=llvm" alt="Clang - support">
  <img src="https://img.shields.io/badge/MSVC-support-5C2D91?style=flat" alt="MSVC - support">
  <img src="https://img.shields.io/badge/AppleClang-support-000000?style=flat&logo=apple" alt="AppleClang - support">
</p>

VectorPro is a header-only, `std::vector`-like dynamic array for modern C++ — a configurable growth policy, full custom-allocator support, a real `std::contiguous_iterator`, and optional zero-overhead modification-event notifications, so you only pay for the parts you actually use.

## 📑 Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Project Structure](#project-structure)
- [Development](#development)
- [Benchmarks](#benchmarks)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Changelog](#changelog)
- [License](#license)

## <a id="features"></a>✨ Features

- **Configurable growth policy** — capacity growth is a compile-time `GrowthNum`/`GrowthDen` ratio (defaults to 2/1, i.e. doubling), not a hardcoded factor. `growCapacity()` saturates at `SIZE_MAX` instead of overflowing, and always guarantees strict growth even for tight ratios.
- **Aliasing-safe `push_back()`/`insert()`** — safe to pass a reference into the vector itself (`v.push_back(v[i])`). The element's offset is snapshotted before any reallocation or shift that could invalidate it, instead of leaving that as undefined behavior for the caller to avoid.
- **Zero-overhead optional event system** — `subscribe()`/`unsubscribe()` are only available on the `ObservableVector` alias. A plain `Vector` with events disabled carries no listener storage at all (`[[no_unique_address]]`) and compiles away every notification call site (`if constexpr` in `notify()`), rather than paying a runtime check per modification.
- **Full custom allocator support** — built on `std::allocator_traits`, honoring `propagate_on_container_copy_assignment`, `propagate_on_container_move_assignment`, and `select_on_container_copy_construction` rather than assuming the default allocator's behavior.
- **A real `std::contiguous_iterator`** — not a raw pointer `typedef`. Supports the full random-access iterator interface, implicit `iterator` → `const_iterator` conversion, and zero-copy `std::span` views via `as_span()`.
- **Exception-safe reallocation and copy paths** — `reallocate()`, `copyBufferFrom()`, and the copy-and-swap fallback in `operator=()` all roll back cleanly on a throwing constructor, leaving the vector exactly as it was before the call.

## <a id="requirements"></a>📋 Requirements

- A C++20-conformant compiler (tested: GCC, Clang, MSVC, AppleClang)
- CMake 3.20+

## <a id="installation"></a>📦 Installation

**From source:**

```bash
git clone https://github.com/privateMwb/VectorPro.git
cd VectorPro
cmake -B build \
  -DBUILD_TESTS=OFF \
  -DBUILD_BENCHMARKS=OFF \
  -DBUILD_REGRESSION=OFF \
  -DBUILD_EXAMPLES=OFF
cmake --install build
```

Then, in your own `CMakeLists.txt`:

```cmake
find_package(VectorPro CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE VectorPro::VectorPro)
```

> vcpkg and Conan packages are built and verified (recipe in
> `packaging/recipes/vectorpro/`, port in `packaging/vcpkg/ports/vectorpro/`),
> but not yet published to the public registries. This section will be
> updated once they are.

## <a id="quick-start"></a>🚀 Quick Start

```cpp
#include <VectorPro/Vector.h>

int main() {
    VectorPro::Vector<int> values;

    values.push_back(1);
    values.push_back(2);
    values.emplace_back(3);

    for (int v : values) {
        // range-for works via begin()/end(), backed by a real
        // std::contiguous_iterator
    }

    values.insert(values.begin(), 0); // {0, 1, 2, 3}
    values.erase(values.begin());     // {1, 2, 3}
}
```

Subscribing to modification events (requires `ObservableVector`):

```cpp
VectorPro::ObservableVector<int> values;

auto handle = values.subscribe([](const auto& v, auto event) {
    std::cout << "size " << event.oldSize << " -> " << event.newSize << '\n';
});

values.push_back(42); // triggers the listener
values.unsubscribe(handle);
```

A custom growth policy and bounds-checked access:

```cpp
VectorPro::Vector<int, std::allocator<int>, /*GrowthNum=*/3, /*GrowthDen=*/2> values;

try {
    values.at(100); // out of range on an empty vector
} catch (const std::out_of_range& e) {
    std::cerr << e.what() << '\n';
}
```

## <a id="project-structure"></a>🗂️ Project Structure

```
VectorPro/
├── include/
│   └── VectorPro/
│       ├── Vector.h
│       ├── Vector.tpp
│       └── Iterator.h
│
├── tests/
│   ├── support/
│   ├── suite/
│   ├── test_main.cpp
│   └── CMakeLists.txt
│
├── benchmarks/
│   ├── support/
│   ├── suite/
│   ├── baselines/
│   ├── bench_main.cpp
│   └── CMakeLists.txt
│
├── examples/
│   ├── support/
│   ├── suite/
│   ├── example_main.cpp
│   └── CMakeLists.txt
│
├── regression/
│   ├── support/
│   ├── regression_main.cpp
│   └── CMakeLists.txt
│
├── packaging/
│   ├── README.md
│   ├── recipes/
│   │   └── vectorpro/
│   ├── vcpkg/
│   │   └── ports/
│   │       └── vectorpro/
│   └── vcpkg-smoke-test/
│
├── scripts/
│   └── update_package_files.py
│
├── .github/
│   ├── releases/
│   └── workflows/
│
├── cmake/
│   └── VectorProConfig.cmake.in
│
├── docs/
│   ├── Doxyfile
│   └── README.md
│
├── .gitignore
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## <a id="development"></a>🛠️ Development

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
./build/regression                  # latest baseline vs. benchmarks/results/benchmark_results.json
./build/regression v1.2.0           # a specific baseline vs. current
./build/regression v1.2.0 v1.4.0    # two baselines against each other
```

`regression` picks the latest baseline by semantic version (`v1.10.0`
correctly outranks `v1.9.0`), not alphabetical filename order, and
auto-names its output (`regression_v1.2.0_vs_current.md`/`.json`, etc.).

See [packaging/README.md](packaging/README.md) for notes on verifying the vcpkg
port and Conan recipe locally.

## <a id="benchmarks"></a>📊 Benchmarks

Measured against `std::vector`, same build, at 10K / 100K / 1M iterations
(`benchmarks/baselines/v1.0.0.json` has the full dataset).

| Operation | VectorPro (1M) | std::vector (1M) | Δ |
|---|---|---|---|
| `Pop Back()` | 63.81 ms | 577.46 ms | +804.9% |
| `Erase() end` | 51.33 ms | 298.78 ms | +482.0% |
| `Clear()` | 63.59 ms | 307.65 ms | +383.8% |
| `Move-construct` | 13.63 ms | 38.59 ms | +183.2% |
| Growth reallocation | 145.45 ms | 386.37 ms | +165.6% |
| `Erase() middle` | 68.23 ms | 175.62 ms | +157.4% |
| `Move-assign` | 16.63 ms | 38.56 ms | +131.9% |
| `Insert() end` | 118.56 ms | 230.92 ms | +94.8% |
| `Front()`/`Back()` | 300.87 us | 508.17 us | +68.9% |
| `Reserve()` no-op (10K) | 5.55 us | 2.75 us | -50.4% |
| Erase-Remove If | 1.31 s | 775.74 ms | -40.8% |

VectorPro's largest wins show up on removal and reallocation-heavy
workloads at the 1M scale — `pop_back()`, `erase()` (front/middle/end),
`clear()`, and growth-driven reallocation all pull well ahead of
`std::vector` as element count grows, and the same holds for move
construction and move assignment.

The trade-off is concentrated in two spots: `remove_if()`'s compaction
loop runs behind `std::vector`'s erase-remove idiom at every scale
tested, and `reserve()`'s no-op check carries a small fixed branch
overhead that only shows up at the smallest (10K) iteration count.

## <a id="documentation"></a>📖 Documentation

Full API reference, generated with Doxygen from `docs/Doxyfile`:

**https://privateMwb.github.io/VectorPro/**

## <a id="contributing"></a>🤝 Contributing

Issues and pull requests are welcome. Before submitting a PR:

- Run the test suite (`ctest --test-dir build`)
- If you're changing a hot path, run `./build/regression` and mention
  the results in your PR description

## <a id="changelog"></a>📝 Changelog

See the [Releases](https://github.com/privateMwb/VectorPro/releases)
page for version history and release notes.

## <a id="license"></a>📄 License

MIT — see [LICENSE](LICENSE) for details.
