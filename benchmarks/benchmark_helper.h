#pragma once

#include "Vector.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

static const int ITERATIONS = 100'000;

inline void bench_header(std::string f) {
    std::string title = f + " Benchmark";
    std::cout << std::string(70, '-') << "\n";
    std::cout << std::left
        << "\033[96m"
        << std::setw(40) << title
        << std::setw(15) << "Time"
        << std::setw(15) << "Iterations"
        << "\033[0m\n";
    std::cout << std::string(70, '-') << "\n";
}

inline void bench_footer() {
    std::cout << std::string(70, '-') << "\n\n";
}

constexpr const char* GREEN   = "\033[92m";
constexpr const char* YELLOW  = "\033[93m";
constexpr const char* RED     = "\033[91m";
constexpr const char* RESET   = "\033[0m";

constexpr auto MS_1   = std::chrono::milliseconds(1);
constexpr auto S_1  = std::chrono::seconds(1);

static inline std::string format_duration(std::chrono::nanoseconds ns) {
    using namespace std::chrono;

    std::ostringstream out;

    if (ns < microseconds(1)) {
        out << ns.count() << " ns";
    }
    else if (ns < milliseconds(1)) {
        out << std::fixed << std::setprecision(2)
            << duration<double, std::micro>(ns).count()
            << " us";
    }
    else if (ns < seconds(1)) {
        out << std::fixed << std::setprecision(2)
            << duration<double, std::milli>(ns).count()
            << " ms";
    }
    else {
        out << std::fixed << std::setprecision(2)
            << duration<double>(ns).count()
            << " s";
    }

    return out.str();
}

static inline std::string timeColor(auto ns, auto raw) {
    std::string colored;
    
    if (ns < MS_1) colored = GREEN + raw + RESET;
    else if (ns < S_1) colored = YELLOW + raw + RESET;
    else colored = RED + raw + RESET;
    
    return colored;
}

static inline std::string iterColor(int iteration) {
    std::string colored;

    if (iteration <= 100'000) colored = GREEN + std::to_string(iteration) + RESET;
    else if (iteration <= 500'000) colored = YELLOW + std::to_string(iteration) + RESET;
    else colored = RED + std::to_string(iteration) + RESET;

    return colored;
}

template<typename T>
inline void doNotOptimize(const T& value) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : "g"(value) : "memory");
#else
    volatile const T* p = &value;
    (void)p;
#endif
}

inline void doNotOptimize() {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" ::: "memory");
#endif
}

#define BENCH(name, iterations, expr)                                               \
do {                                                                                \
    std::ostringstream stream;                                                      \
    auto start = std::chrono::high_resolution_clock::now();                         \
    for (int i = 0; i < iterations; ++i) { (void)(expr); doNotOptimize(); }         \
    auto end = std::chrono::high_resolution_clock::now();                           \
    auto ns  = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);   \
    auto raw = format_duration(ns);                                             \
    std::string time  = timeColor(ns, raw);                                         \
    std::string num   = iterColor(iterations);                                      \
    std::cout << std::left                                                          \
              << std::setw(40) << name                                              \
              << std::setw(25) << time                                              \
              << std::setw(20) << num                                               \
              << "\n";                                                              \
} while (0)

// Shared Callable 

// pre-fills a vector with n integers
static const std::size_t BENCH_SIZE = 100'000;

inline void fillVector(VectorPro::Vector<int>& v, std::size_t n = BENCH_SIZE) {
    for (std::size_t i = 0; i < n; ++i)
        v.push_back(static_cast<int>(i));
}

// pre-fills a std::vector for comparison benchmarks
inline void fillStd(std::vector<int>& v, std::size_t n = BENCH_SIZE) {
    for (std::size_t i = 0; i < n; ++i)
        v.push_back(static_cast<int>(i));
}

// lightweight listener for observer benchmarks
inline int g_eventCount = 0;

inline void countListener(
    const VectorPro::Vector<int>&,
    VectorPro::Vector<int>::EventData) {
    ++g_eventCount;
}
