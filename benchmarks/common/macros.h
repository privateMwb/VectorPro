#include "helper.h"

// Executes and times a benchmark expression. Still records into
// benchmark_results() exactly as before, but no longer prints — printing
// now happens once per comparison pair, in printComparisonRow(), so both
// sides land on the same line instead of two separate ones.
// outNs receives the elapsed duration so the caller (BENCH) can use it.
#define BENCHMARK(name, iteration, expr, outNs, custom)                                            \
    do {                                                                                           \
        auto start = steady_clock::now();                                                          \
        for (std::size_t i = 0; i < iteration; ++i) {                                              \
            doNotOptimize();                                                                       \
            expr();                                                                                \
            doNotOptimize();                                                                       \
        }                                                                                          \
        auto end = steady_clock::now();                                                            \
                                                                                                   \
        (outNs) = duration_cast<nanoseconds>(end - start);                                         \
                                                                                                   \
        if (custom)                                                                                \
            benchmark_results().push_back(                                                         \
                {getSuite(), name, static_cast<std::uint64_t>((outNs).count()), iteration,         \
                 static_cast<double>((outNs).count()) / static_cast<double>(iteration)});          \
    } while (0)

#define BENCH(name, c_expr, s_expr)                                                                \
    do {                                                                                           \
        nanoseconds cNs{}, sNs{};                                                                  \
                                                                                                   \
        BENCHMARK(std::string(name), SMALL, c_expr, cNs, true);                                    \
        BENCHMARK(std::string(name), SMALL, s_expr, sNs, false);                                   \
        printComparisonRow(name, "10K", cNs, sNs);                                                 \
                                                                                                   \
        BENCHMARK(std::string(name), MEDIUM, c_expr, cNs, true);                                   \
        BENCHMARK(std::string(name), MEDIUM, s_expr, sNs, false);                                  \
        printComparisonRow(name, "100K", cNs, sNs);                                                \
                                                                                                   \
        BENCHMARK(std::string(name), LARGE, c_expr, cNs, true);                                    \
        BENCHMARK(std::string(name), LARGE, s_expr, sNs, false);                                   \
        printComparisonRow(name, "1M", cNs, sNs);                                                  \
    } while (0)

// For benchmarks with no std::vector equivalent (no comparison, no delta).
// Reuses BENCHMARK exactly like BENCH does — custom=true so it lands in
// benchmark_results() / the JSON export the same way VectorPro's side of
// a normal BENCH pair does.
#define BENCH_SOLO(name, expr)                                                                     \
    do {                                                                                           \
        nanoseconds ns{};                                                                          \
                                                                                                   \
        BENCHMARK(std::string(name), SMALL, expr, ns, true);                                       \
        printSoloRow(name, "10K", ns);                                                             \
                                                                                                   \
        BENCHMARK(std::string(name), MEDIUM, expr, ns, true);                                      \
        printSoloRow(name, "100K", ns);                                                            \
                                                                                                   \
        BENCHMARK(std::string(name), LARGE, expr, ns, true);                                       \
        printSoloRow(name, "1M", ns);                                                              \
    } while (0)

// Registers this file's run_benchmarks with the global registry so it
// runs automatically at startup. The anonymous namespace gives
// `registrar` internal linkage, preventing duplicate-symbol errors when
// this macro is expanded in multiple translation units.
#define REGISTER_BENCH_SUITE()                                                                     \
    namespace {                                                                                    \
    static BenchRegistrar registrar(__FILE__, run_benchmarks);                                     \
    }