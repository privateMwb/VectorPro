# Google Benchmark Suite

This document describes the benchmark categories under `suite/` — what each
one measures, and the individual benchmarks it contains. Same categories as
`../custom/suite/`, reimplemented with Google Benchmark instead of the
custom framework.

| Category | Focus |
|---|---|
| [Access](#access) | Read-only access and traversal against an already-populated vector |
| [Core](#core) | Appending, inserting, erasing, and draining |
| [Lifecycle](#lifecycle) | Construction, copying, and moving |
| [Scaling](#scaling) | Reserving, growing, and shrinking the underlying buffer |
| [Utility](#utility) | Comparison, change-notification overhead, and swapping |
| [Conventions](#conventions) | Naming pattern and macro gotchas specific to Google Benchmark |

Every benchmark pairs `VectorPro::Vector` against `std::vector`, one
`BENCHMARK()` function per implementation, except `observer.cpp`: there's no
`std::vector` equivalent for change notification, so those benchmarks time
`ObservableVector` alone.

Iteration count is handled by Google Benchmark itself — each `BENCHMARK()`
runs until `--benchmark_min_time` is satisfied, the Google Benchmark
equivalent of the custom suite's SMALL/MEDIUM/LARGE tiers, without needing
to register separate sizes by hand.

Every benchmark auto-registers via `BENCHMARK(...)` at startup — no suite
list to maintain by hand. Benchmark names double as the filter you'd pass to
`--benchmark_filter`, e.g. `--benchmark_filter=Insert` runs everything with
"Insert" in its name. This applies uniformly across every category below.

---

## Access

Benchmarks read-only access and traversal against an already-populated
vector.

### Benchmarks

| File | What it covers |
|---|---|
| `element_access.cpp` | Single-element accessors: `At`, `FrontBack`, `DataPtr` (each `_VectorPro`/`_StdVector`) |
| `iteration.cpp` | Full traversal: `RangeFor`, `IteratorLoop`, `IndexLoop`, `ReverseIteration`, `Accumulate` (each `_VectorPro`/`_StdVector`) |
| `search.cpp` | `contains()`/`find()` vs. `std::find()`: `ContainsHitSmall`, `ContainsMissSmall`, `ContainsHitLarge`, `FindHitSmall`, `FindMissSmall` (each `_VectorPro`/`_StdVector`) |

---

## Core

Benchmarks the fundamental, most frequently exercised mutating operations —
appending, inserting, erasing, and draining.

### Benchmarks

| File | What it covers |
|---|---|
| `erase.cpp` | `erase()` at each position: `EraseFront`, `EraseMiddle`, `EraseEnd` (each `_VectorPro`/`_StdVector`) |
| `insert.cpp` | `insert()` at each position: `InsertFront`, `InsertMiddle`, `InsertEnd` (each `_VectorPro`/`_StdVector`) |
| `pop_clear.cpp` | `pop_back()` and `clear()`: `PopBack`, `Clear` (each `_VectorPro`/`_StdVector`) |
| `push_back.cpp` | `push_back()`/`emplace_back()`: `PushBack`, `PushBackReserved`, `EmplaceBack` (each `_VectorPro`/`_StdVector`) |
| `remove_if.cpp` | `remove_if()` vs. the erase-remove idiom: `EraseRemoveIf` (`_VectorPro`/`_StdVector`) |

---

## Lifecycle

Benchmarks object lifetime operations — construction, copying, and moving.

### Benchmarks

| File | What it covers |
|---|---|
| `construction.cpp` | `DefaultConstruct`, `FillConstruct`, `InitListConstruct` (each `_VectorPro`/`_StdVector`) |
| `copy.cpp` | `CopyConstruct`, `CopyAssignment` (each `_VectorPro`/`_StdVector`) |
| `move.cpp` | `MoveConstruct`, `MoveAssignment` (each `_VectorPro`/`_StdVector`) |

---

## Scaling

Benchmarks storage-management operations — reserving, growing, and
shrinking the underlying buffer.

### Benchmarks

| File | What it covers |
|---|---|
| `reallocation.cpp` | Cost of a single growth-triggered reallocation on a populated buffer: `GrowthReallocation` (each `_VectorPro`/`_StdVector`) |
| `reserve.cpp` | `Reserve`, `ReserveGrowth`, `ReserveNoop` (each `_VectorPro`/`_StdVector`) |
| `shrink.cpp` | `shrink_to_fit()` after over-reserving: `ShrinkToFit` (each `_VectorPro`/`_StdVector`) |

---

## Utility

Benchmarks small, self-contained operations that don't belong to any of the
categories above — relational comparison, change-notification overhead, and
swapping.

### Benchmarks

| File | What it covers |
|---|---|
| `comparison.cpp` | `operator==` and `operator<=>`: `Equal_True`, `Equal_DiffersEarly`, `Equal_DiffersLate`, `Equal_DifferentSizes`, `Spaceship_Equal`, `Spaceship_Less` (each `_VectorPro`/`_StdVector`) |
| `observer.cpp` | `ObservableVector` change-notification overhead, no `std::vector` baseline: `PushBack_NoListeners`, `PushBack_OneListener`, `PushBack_ManyListeners`, `PushBack_CapturingListener`, `Subscribe_x100`, `Unsubscribe_x100` |
| `swap.cpp` | Member vs. free-function swap: `MemberSwap`, `FreeSwap` (each `_VectorPro`/`_StdVector`) |

---

## Conventions

- **Paired case** — two functions per operation, `BM_<Name>_VectorPro` and
  `BM_<Name>_StdVector`, each registered with its own `BENCHMARK(...)` call
  so they sort together in the results table under a shared name prefix.
- **Solo case** — a single function with no `_StdVector` counterpart (used
  only in `observer.cpp`, where `std::vector` has no equivalent).
- All benchmarks build into a single binary and run through the standard
  `BENCHMARK_MAIN()` entry point — no custom suite registration.
- `benchmark::DoNotOptimize(...)` is used in place of the old
  `doNotOptimize(...)`. For anything larger than a register (arrays,
  structs, containers), pass a pointer (`.data()`, `&x`) rather than the
  object itself — passing the object by value/const-ref trips a
  deprecation warning in newer Google Benchmark versions and is a weaker
  optimization barrier.
- Use `--benchmark_filter=<regex>` to run a subset, and `--benchmark_out=<file>
  --benchmark_out_format=json` to capture results for comparison across
  runs (e.g. with `compare.py` from the Google Benchmark tooling).
