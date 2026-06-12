// VectorPro vs std::vector Benchmark Suite
// Measures core dynamic array operations:
//
// - push_back without reserve (amortized growth + reallocation cost)
// - push_back with reserve (pure append cost, no reallocation)
// - emplace_back (in-place construction efficiency)
// - pop_back (O(1) removal from end)
// - insert front (worst-case O(n) shifting)
// - insert middle (average-case O(n) shifting)
// - erase front (worst-case O(n) shifting)
// - erase middle (average-case O(n) shifting)
// - remove_if (repeated fill and filter cycles)
// - iteration (range-based traversal, cache efficiency)
// - copy construction (deep copy cost)
// - move construction (ownership transfer cost)
//
// Benchmarks compare VectorPro with std::vector.

#include <iostream>
#include <chrono>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <string>

#include "VectorPro.h"
#include "utils/Table.h"

// returns elapsed microseconds for a callable
template<typename F>
auto duration(F func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end   = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

// prevents the compiler from eliminating unused operations
template<typename T>
inline void doNotOptimize(T* ptr) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : "r,m"(ptr) : "memory");
#else
    volatile T* v = ptr;
    (void)v;
#endif
}

// Push Back (No Reserve)
// measures amortized append including reallocation cost
void pushBack() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        500'000,
        1'000'000,
        2'000'000,
        4'000'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        auto vp_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j) {
                vp.push_back(static_cast<int>(j));
                doNotOptimize(&vp[j]);
            }
        });

        auto v_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j) {
                v.push_back(static_cast<int>(j));
                doNotOptimize(&v[j]);
            }
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Push Back - No Reserve",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Push Back (With Reserve)
// measures pure append cost with reallocation eliminated
void pushBackReserved() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        500'000,
        1'000'000,
        2'000'000,
        4'000'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        vp.reserve(counts[i]);
        v.reserve(counts[i]);

        auto vp_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j) {
                vp.push_back(static_cast<int>(j));
                doNotOptimize(&vp[j]);
            }
        });

        auto v_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j) {
                v.push_back(static_cast<int>(j));
                doNotOptimize(&v[j]);
            }
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Push Back - With Reserve",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Emplace Back
// measures in-place construction vs push_back overhead
void emplaceBack() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        500'000,
        1'000'000,
        2'000'000,
        4'000'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        vp.reserve(counts[i]);
        v.reserve(counts[i]);

        auto vp_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j) {
                vp.emplace_back(static_cast<int>(j));
                doNotOptimize(&vp[j]);
            }
        });

        auto v_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j) {
                v.emplace_back(static_cast<int>(j));
                doNotOptimize(&v[j]);
            }
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Emplace Back",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Pop Back
// measures O(1) removal from end
void popBack() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        500'000,
        1'000'000,
        2'000'000,
        4'000'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        for (std::size_t j = 0; j < counts[i]; ++j) {
            vp.push_back(static_cast<int>(j));
            v.push_back(static_cast<int>(j));
        }

        auto vp_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j)
                vp.pop_back();
        });

        auto v_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j)
                v.pop_back();
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Pop Back",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Insert Front
// measures worst-case O(n) shifting — always inserts at index 0
void insertFront() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        5'000,
        10'000,
        20'000,
        40'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        auto vp_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j)
                vp.insert(0, static_cast<int>(j));
        });

        auto v_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j)
                v.insert(v.begin(), static_cast<int>(j));
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Insert Front - Worst Case",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Insert Middle
// measures average-case O(n) shifting — always inserts at midpoint
void insertMiddle() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        5'000,
        10'000,
        20'000,
        40'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        for (std::size_t j = 0; j < counts[i]; ++j) {
            vp.push_back(static_cast<int>(j));
            v.push_back(static_cast<int>(j));
        }

        auto vp_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j) {
                std::size_t mid = vp.size() / 2;
                vp.insert(mid, static_cast<int>(j));
            }
        });

        auto v_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j) {
                auto mid = v.begin() + v.size() / 2;
                v.insert(mid, static_cast<int>(j));
            }
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Insert Middle - Average Case",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Erase Front
// measures worst-case O(n) shifting — always erases index 0
void eraseFront() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        5'000,
        10'000,
        20'000,
        40'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        for (std::size_t j = 0; j < counts[i]; ++j) {
            vp.push_back(static_cast<int>(j));
            v.push_back(static_cast<int>(j));
        }

        auto vp_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j)
                vp.erase(0);
        });

        auto v_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i]; ++j)
                v.erase(v.begin());
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Erase Front - Worst Case",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Erase Middle
// measures average-case O(n) shifting — always erases midpoint
void eraseMiddle() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        5'000,
        10'000,
        20'000,
        40'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        for (std::size_t j = 0; j < counts[i]; ++j) {
            vp.push_back(static_cast<int>(j));
            v.push_back(static_cast<int>(j));
        }

        auto vp_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i] / 2; ++j)
                vp.erase(vp.size() / 2);
        });

        auto v_duration = duration([&] {
            for (std::size_t j = 0; j < counts[i] / 2; ++j)
                v.erase(v.begin() + v.size() / 2);
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Erase Middle - Average Case",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Remove If
// measures repeated fill and filter cycles for stable timing
void removeIf() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        100'000,
        200'000,
        400'000,
        800'000
    };

    static constexpr std::size_t CYCLES = 5;

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<long> vp_cycle;
        VectorPro<long> v_cycle;

        for (std::size_t c = 0; c < CYCLES; ++c) {
            VectorPro<int>   vp;
            std::vector<int> v;

            for (std::size_t j = 0; j < counts[i]; ++j) {
                vp.push_back(static_cast<int>(j));
                v.push_back(static_cast<int>(j));
            }

            auto vp_duration = duration([&] {
                vp.remove_if([](int x) { return x % 2 == 0; });
            });

            auto v_duration = duration([&] {
                v.erase(
                    std::remove_if(v.begin(), v.end(),
                        [](int x) { return x % 2 == 0; }),
                    v.end());
            });

            vp_cycle.push_back(vp_duration.count());
            v_cycle.push_back(v_duration.count());
        }

        // average across cycles
        long vp_avg = 0;
        long v_avg  = 0;

        for (std::size_t c = 0; c < CYCLES; ++c) {
            vp_avg += vp_cycle[c];
            v_avg  += v_cycle[c];
        }

        vp_durations.push_back(vp_avg / static_cast<long>(CYCLES));
        v_durations.push_back(v_avg  / static_cast<long>(CYCLES));
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Remove If  (avg of 5 cycles)",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Iteration
// measures range-based for loop traversal and cache efficiency
void iteration() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        1'000'000,
        2'000'000,
        4'000'000,
        8'000'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        vp.reserve(counts[i]);
        v.reserve(counts[i]);

        for (std::size_t j = 0; j < counts[i]; ++j) {
            vp.push_back(static_cast<int>(j));
            v.push_back(static_cast<int>(j));
        }

        auto vp_duration = duration([&] {
            for (const auto& val : vp)
                doNotOptimize(const_cast<int*>(&val));
        });

        auto v_duration = duration([&] {
            for (const auto& val : v)
                doNotOptimize(const_cast<int*>(&val));
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Iteration",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Copy Construction
// measures deep copy cost at various sizes
void copyConstruct() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        500'000,
        1'000'000,
        2'000'000,
        4'000'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        for (std::size_t j = 0; j < counts[i]; ++j) {
            vp.push_back(static_cast<int>(j));
            v.push_back(static_cast<int>(j));
        }

        auto vp_duration = duration([&] {
            VectorPro<int> copy(vp);
            doNotOptimize(copy.data_ptr());
        });

        auto v_duration = duration([&] {
            std::vector<int> copy(v);
            doNotOptimize(copy.data());
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Copy Construction",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Move Construction
// measures ownership transfer cost — should be O(1) flat regardless of size
void moveConstruct() {
    VectorPro<long> vp_durations;
    VectorPro<long> v_durations;

    VectorPro<std::size_t> counts = {
        500'000,
        1'000'000,
        2'000'000,
        4'000'000
    };

    for (std::size_t i = 0; i < counts.size(); ++i) {
        VectorPro<int>   vp;
        std::vector<int> v;

        for (std::size_t j = 0; j < counts[i]; ++j) {
            vp.push_back(static_cast<int>(j));
            v.push_back(static_cast<int>(j));
        }

        auto vp_duration = duration([&] {
            VectorPro<int> moved(std::move(vp));
            doNotOptimize(moved.data_ptr());
            vp = std::move(moved);
        });

        auto v_duration = duration([&] {
            std::vector<int> moved(std::move(v));
            doNotOptimize(moved.data());
            v = std::move(moved);
        });

        vp_durations.push_back(vp_duration.count());
        v_durations.push_back(v_duration.count());
    }

    VectorPro<VectorPro<std::string>> data{
        Table::convert(counts),
        Table::convert(vp_durations, "us"),
        Table::convert(v_durations,  "us")
    };

    Table::table(
        "Move Construction",
        { "Count", "VectorPro", "std::vector" },
        data, 56);
}

// Entry Point
int main() {
    pushBack();
    pushBackReserved();
    emplaceBack();
    popBack();
    insertFront();
    insertMiddle();
    eraseFront();
    eraseMiddle();
    removeIf();
    iteration();
    copyConstruct();
    moveConstruct();

    return 0;
}

