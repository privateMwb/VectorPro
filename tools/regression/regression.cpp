// clang-format off
#include "export.h" // exportJson, exportMarkdown
#include "helper.h" // loadResults, setHeader, printComparisonRow, convertIter, getCns, BenchmarkResult
// clang-format on

// Iterates baseline results, printing/recording a comparison row against
// current results for each entry, grouped and re-headered by suite.
static void printRegression(const VectorPro::Vector<BenchmarkResult>& baseline,
                            const VectorPro::Vector<BenchmarkResult>& current) {
    std::string currentSuite = " ";

    for (std::size_t i = 0; i < baseline.size(); ++i) {
        std::string suite = baseline[i].suite;

        if (currentSuite != suite) {
            std::cout << "\n";
            setHeader(suite);
            currentSuite = suite;
        }

        std::string op = baseline[i].operation;
        std::size_t iter = baseline[i].iterations;
        std::string citer = convertIter(iter);
        double bns = baseline[i].ns_per_op;
        double cns = getCns(current, op, iter);

        printComparisonRow(suite, op, iter, citer, bns, cns);

        if (citer == "1M")
            std::cout << "\n";
    }
}

// Loads baseline + current benchmark snapshots, prints the regression
// comparison, and exports the results as JSON and markdown reports.
int main() {
    try {
        auto baselineResults = loadResults("benchmarks/baselines/v1.0.0.json");
        auto currentResults = loadResults("benchmarks/results/benchmark_results.json");

        printRegression(baselineResults, currentResults);

        exportJson("regression_results.json");
        exportMarkdown("regression_results.md");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
