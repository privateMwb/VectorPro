#include "export.h"
#include "helper.h"

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
