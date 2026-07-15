#pragma once

#include <VectorPro/Vector.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std::chrono;

// ANSI terminal color codes.
constexpr const char* RESET = "\033[0m";
constexpr const char* GREEN = "\033[92m";
constexpr const char* YELLOW = "\033[93m";
constexpr const char* RED = "\033[91m";
constexpr const char* CYAN = "\033[96m";
constexpr const char* GRAY = "\033[37m";

// Benchmark iteration presets.
constexpr const std::size_t SMALL = 10'000;
constexpr const std::size_t MEDIUM = 100'000;
constexpr const std::size_t LARGE = 1'000'000;

inline std::string custom = "VectorPro";
inline std::string standard = "std::vector";
inline std::string suiteName;

// Accumulates a markdown-formatted transcript of the run, built up by
// setHeader()/printComparisonRow() alongside their normal stdout printing.
// Kept here (not reconstructed later from benchmark_results()) because
// section grouping and VectorPro/std::vector pairing only exist at print
// time — benchmark_results() stores a flat, unpaired, ungrouped list.
inline std::string& markdown_buffer() {
    static std::string buffer;
    return buffer;
}

// Prints a horizontal separator line.
inline void borderLine() {
    std::cout << GRAY << std::string(90, '-') << RESET << "\n";
}

inline void setSuite(const std::string name) {
    suiteName = name;
}

inline const std::string getSuite() {
    return suiteName;
}

// Formats a duration using the most appropriate time unit.
inline auto formatDuration(nanoseconds ns) {
    std::ostringstream out;

    if (ns < microseconds(1))
        out << ns.count() << " ns";
    else if (ns < milliseconds(1))
        out << std::fixed << std::setprecision(2) << duration<double, std::micro>(ns).count()
            << " us";
    else if (ns < seconds(1))
        out << std::fixed << std::setprecision(2) << duration<double, std::milli>(ns).count()
            << " ms";
    else
        out << std::fixed << std::setprecision(2) << duration<double>(ns).count() << " s";

    return out.str();
}

// Converts a snake_case function name to Title Case.
inline std::string prettify(std::string_view text) {
    std::string result{text};
    bool firstLetter = true;

    for (char& c : result) {
        if (firstLetter) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            firstLetter = false;
        } else if (c == '_') {
            c = ' ';
            firstLetter = true;
        }
    }

    return result;
}

// Prevents the compiler from optimizing away benchmarked values.
template <typename T> inline void doNotOptimize(const T& value) {
#if defined(__GNUC__) || defined(__clang__)
    // "g" constraint forces the value into a register/memory and the
    // "memory" clobber stops the compiler from reordering or eliding
    // the computation that produced it.
    asm volatile("" : : "g"(value) : "memory");
#else
    // Weaker fallback for compilers without inline asm support: taking
    // a volatile pointer discourages (but doesn't guarantee against)
    // the optimizer dropping the computation.
    volatile const T* p = &value;
    (void)p;
#endif
}

// Prevents the compiler from optimizing away benchmarked code.
inline void doNotOptimize() {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" ::: "memory");
#endif
}

// Transform string case to lower case.
inline std::string toLower(std::string_view str) {
    std::string result(str);

    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    return result;
}

// Prints a 3-column table header for benchmarks with no std::vector
// equivalent (nothing to diff against, so no Baseline/Δ columns).
inline void setSoloHeader(std::string_view header) {
    borderLine();
    std::cout << std::left << CYAN << std::setw(30) << prettify(header) << std::setw(15)
              << "Iteration" << std::setw(15) << custom << RESET << "\n";
    borderLine();

    markdown_buffer() += "\n## " + prettify(header) + "\n\n";
    markdown_buffer() += "| Test | Iteration | " + custom + " |\n";
    markdown_buffer() += "|---|---|---|\n";
}

// Prints one solo row: name, iteration tier, single duration. No delta,
// no color — there's nothing to compare against.
// clang-format off
inline void printSoloRow(std::string_view name, std::string_view iteration, nanoseconds ns) {
    std::cout << std::left 
              << std::setw(30) << prettify(name) 
              << std::setw(15) << iteration
              << std::setw(15) << formatDuration(ns) 
              << std::setw(15) << "     —" 
              << std::setw(15) << "    —" 
              << "\n";

    markdown_buffer() += "| " + prettify(name) 
                      + " | " + std::string(iteration) 
                      + " | " + formatDuration(ns) 
                      + " |\n";
}
// clang-format on

// Prints the benchmark table header.
inline void setHeader(std::string_view header) {
    borderLine();
    std::cout << std::left << CYAN << std::setw(30) << prettify(header) << std::setw(15)
              << "Iteration" << std::setw(15) << custom << std::setw(15) << standard
              << std::setw(15) << "  Δ" << RESET << "\n";
    borderLine();

    markdown_buffer() += "\n## " + prettify(header) + "\n\n";
    markdown_buffer() += "| Test | Iteration | " + custom + " | " + standard + " | Δ |\n";
    markdown_buffer() += "|---|---|---|---|---|\n";
}

// Prints one merged comparison row: name, iteration tier, both durations,
// and the % delta (VectorPro vs std::vector) colorized — red if VectorPro
// is meaningfully slower, green if meaningfully faster, gray within the
// noise threshold. Replaces printing each side as its own separate line.
inline void printComparisonRow(std::string_view name, std::string_view iteration,
                               nanoseconds customNs, nanoseconds stdNs) {
    const double pct =
        stdNs.count() == 0
            ? 0.0
            : (static_cast<double>(stdNs.count()) - static_cast<double>(customNs.count())) /
                  static_cast<double>(customNs.count()) * 100.0;

    // +-15% treated as measurement noise, not a real signal — see the
    // Termux big.LITTLE / thermal-throttling discussion.
    const char* deltaColor = (pct > 0.0) ? GREEN : (pct < 0.0) ? RED : GRAY;

    std::ostringstream deltaStream;
    deltaStream << std::showpos << std::fixed << std::setprecision(1) << pct << "%";

    std::cout << std::left << std::setw(30) << prettify(name) << std::setw(15) << iteration
              << std::setw(15) << formatDuration(customNs) << std::setw(15) << formatDuration(stdNs)
              << deltaColor << deltaStream.str() << RESET << "\n";

    markdown_buffer() += "| " + prettify(name) + " | " + std::string(iteration) + " | " +
                         formatDuration(customNs) + " | " + formatDuration(stdNs) + " | " +
                         deltaStream.str() + " |\n";
}