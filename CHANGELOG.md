# Changelog

All notable changes to VectorPro are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Nothing yet.

## [1.0.0] - 2026-07-31

The first stable release of VectorPro, a configurable, header-only dynamic
array for modern C++20.

### Added
- Contiguous, random-access iterator satisfying `std::contiguous_iterator`.
- Configurable growth policy via `GrowthNum`/`GrowthDen` template parameters.
- Custom allocator support through `std::allocator_traits`.
- `push_back()`, `emplace_back()`, `insert()`, and `emplace()` with
  aliasing-safe handling of self-referencing values during reallocation.
- `erase()`, `remove_if()`, `pop_back()`, and `clear()` for element removal.
- `reserve()` and `shrink_to_fit()` for explicit capacity management.
- Copy and move construction/assignment, allocator-aware and exception-safe.
- `operator==` and `operator<=>` for full lexicographical comparison.
- `as_span()` for zero-copy `std::span` views over the underlying storage.
- `contains()` and `find()` lookups.
- Member and free `swap()`.
- Iterator-pair and initializer-list construction.
- Optional, zero-overhead modification-event notifications via
  `subscribe()`/`unsubscribe()`, enabled through the `ObservableVector` alias.
- `rain::` namespace alias for both `Vector` and `Iterator`.

### Performance
- `memcpy`-based fast paths for trivially copyable types with the default
  allocator in `reallocate()`, `copyBufferFrom()`, copy assignment, and
  `operator==`.
- Saturating growth calculation (`growCapacity()`) avoids overflow on
  repeated doubling and guarantees strict capacity growth even under
  unusual growth ratios.
- `[[no_unique_address]]` listener storage means a plain `Vector` (events
  disabled) carries zero bytes of bookkeeping for the notification system —
  the `if constexpr` in `notify()` compiles the call away entirely.
- Cold/hot path split in `push_back()`: the common no-growth case stays
  small enough to inline, while the reallocation path is factored out
  separately.
- Benchmarked against `std::vector` at 10K / 100K / 1M iterations; largest
  wins on removal and reallocation-heavy workloads at the 1M scale
  (`pop_back()`, `erase()`, `clear()`, growth reallocation, move
  construction/assignment). Full results in `benchmarks/results/v1_0_0.md`.

### Testing
- Comprehensive test suite covering unit, integration, lifecycle,
  regression, and concurrency tests; move semantics; copy/move allocator
  propagation; growth policy and reallocation behavior; iterator
  conformance; insert/erase aliasing; `remove_if()` compaction
  correctness; event notification behavior; comparison operators; and
  exception safety / strong-guarantee paths.
- 95.7% line coverage (462/483 lines) and 99.6% function coverage
  (233/234 functions), excluding test infrastructure.

### CI
- Automated builds and tests across GCC, Clang, MSVC, and AppleClang, each
  in Debug and Release configurations.

[Unreleased]: https://github.com/privateMwb/VectorPro/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/privateMwb/VectorPro/releases/tag/v1.0.0
