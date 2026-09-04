# Example Suite

This document describes the example categories under `suite/` — what each
one demonstrates, and the individual example files it contains.

| Category | Focus |
|---|---|
| [Advanced](#advanced) | Allocator support, move-only elements, and growth policies |
| [Integration](#integration) | Interoperability with the rest of a codebase |
| [Misuse](#misuse) | Common mistakes and the exceptions/UB they lead to |
| [Patterns](#patterns) | Common usage idioms built on the core API |
| [Quickstart](#quickstart) | Fundamental, everyday usage |

Unlike the test suite, an example doesn't assert correctness — it
demonstrates real usage of the library, including deliberate misuse where
instructive (see Misuse), so the reader sees both the correct pattern and the
mistake it guards against.

Every example file ends with `REGISTER_EXAMPLE_SUITE()`, which derives the
suite's category from its containing directory and assigns it a sequential id
within that category. This applies uniformly across every category below.

---

## Advanced

Demonstrates deeper mechanics of the library — allocator support beyond the
default, move-only element types, and the observable effects of different
growth policies.

### Examples

| File | What it covers |
|---|---|
| `allocator.cpp` | `std::pmr::polymorphic_allocator`, a custom stateful counting allocator, and allocator behavior on copy construction |
| `growth.cpp` | The default 2/1 growth factor against custom 3/2 and 4/1 policies, and the reallocation-count/wasted-capacity trade-off between them |
| `move_only.cpp` | Storing `std::unique_ptr`, `push_back()`/`emplace_back()` with move-only elements, move-constructing a Vector of them, and moving a single element out |

---

## Integration

Demonstrates interoperability with the rest of a codebase — the standard
algorithms and ranges libraries, `std::span`, and round-tripping through
`<sstream>`.

### Examples

| File | What it covers |
|---|---|
| `algorithms.cpp` | `std::sort`, `std::transform`, `std::accumulate`, `std::ranges` algorithms, and `as_span()` interop with `std::span` |
| `serialize.cpp` | Writing a `Vector<int>` out through `std::ostringstream`, reading it back via `std::istringstream`, and a round-trip equality check |

---

## Misuse

Demonstrates common mistakes and the exceptions or undefined behavior they
lead to, alongside the correct pattern — including examples shown but not
executed, so the reader can see what to avoid without the program actually
invoking undefined behavior.

### Examples

| File | What it covers |
|---|---|
| `bounds.cpp` | `at()` throwing `std::out_of_range` on an invalid index; `operator[]`'s lack of bounds checking; the size()-as-last-index off-by-one; the correct bounds-check-first pattern |
| `invalidation.cpp` | Iterators/pointers invalidated by a growth-triggering `push_back()`, by `insert()`, and by `erase()`; the correct pattern of re-fetching iterators after a mutating call |

---

## Patterns

Demonstrates common usage idioms built on top of the core API — storing
non-trivial user types, the full modifier set, and reacting to mutation
events.

### Examples

| File | What it covers |
|---|---|
| `custom_type.cpp` | A non-trivial `Employee` struct, construction and deep-copy semantics, and sorting with a custom comparator |
| `modifiers.cpp` | `insert()` at front/middle/end, `erase()` of a single element and a range, `emplace()`, `remove_if()`, `pop_back()`, `clear()` |
| `observer.cpp` | Subscribing to mutation events, reading event payloads, multiple listeners, and unsubscribing |

---

## Quickstart

Demonstrates fundamental, everyday usage — construction, growth, element
access, iteration, and capacity management.

### Examples

| File | What it covers |
|---|---|
| `basic.cpp` | Initializer-list construction, `push_back()` growth, `front()`/`back()`/`operator[]`/`at()`, range-based iteration, `reserve()`/`shrink_to_fit()` |
