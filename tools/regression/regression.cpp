#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

// ANSI terminal color codes.
constexpr const char* RESET = "\033[0m";
constexpr const char* GREEN = "\033[92m";
constexpr const char* RED = "\033[91m";
constexpr const char* YELLOW = "\033[93m";
constexpr const char* CYAN = "\033[96m";
constexpr const char* GRAY = "\033[37m";
constexpr const char* BLUE = "\033[94m";

struct BenchmarkResult
{
    std::string name;
    double ns_per_op;
};


static std::vector<BenchmarkResult>
loadResults(const std::string& file)
{
    std::ifstream in(file);

    if (!in)
    {
        throw std::runtime_error("Failed to open: " + file);
    }

    json data;
    in >> data;

    std::vector<BenchmarkResult> results;

    for (const auto& entry : data)
    {
        // Only load VectorPro benchmarks
        if (!entry.contains("library"))
            continue;

        if (entry["library"] != "VectorPro")
            continue;

        results.push_back({
            entry["name"],
            entry["ns_per_op"]
        });
    }

    return results;
}


static std::unordered_map<std::string, double>
toMap(const std::vector<BenchmarkResult>& results)
{
    std::unordered_map<std::string, double> map;

    for (const auto& result : results)
    {
        map[result.name] = result.ns_per_op;
    }

    return map;
}


static void printRegression(
    const std::unordered_map<std::string, double>& baseline,
    const std::unordered_map<std::string, double>& current)
{
    constexpr double warningThreshold = 5.0;
    constexpr double failureThreshold = 10.0;


    std::cout << BLUE << "\nRegression Report\n\n" << RESET ;


    for (const auto& [name, currentTime] : current)
    {
        auto it = baseline.find(name);

        if (it == baseline.end())
            continue;


        double oldTime = it->second;


        // Positive means slower
        double change =
            ((currentTime - oldTime) / oldTime) * 100.0;


        std::cout << CYAN << name << RESET << "\n";

        std::cout
            << "  Baseline : "
            << oldTime
            << " ns/op\n";


        std::cout
            << "  Current  : "
            << currentTime
            << " ns/op\n";


        if (change > failureThreshold)
        {
            std::cout
                << RED 
                << "  FAILURE  "
                << RESET
                << ": "
                << RED
                << std::fixed
                << std::setprecision(2)
                << change
                << "% slower\n"
                << RESET;
        }
        else if (change > warningThreshold)
        {
            std::cout
                << YELLOW
                << "  Warning  "
                << RESET
                << ": "
                << YELLOW
                << std::fixed
                << std::setprecision(2)
                << change
                << "% slower\n"
                << RESET;
        }
        else if (change < -warningThreshold)
        {
            std::cout
                << GREEN 
                << "  Change   "
                << RESET
                << ": "
                << GREEN
                << std::fixed
                << std::setprecision(2)
                << -change
                << "% faster\n"
                << RESET;
        }
        else
        {
            std::cout
                << GRAY
                << "  Stable   "
                << RESET
                << ": "
                << GRAY
                << std::fixed
                << std::setprecision(2)
                << change
                << "%\n"
                << RESET;
        }


        std::cout << "\n";
    }
}



int main()
{
    try
    {
        auto baselineResults =
            loadResults("benchmarks/baselines/v1.0.0.json");

        auto currentResults =
            loadResults("benchmark_results.json");


        auto baseline =
            toMap(baselineResults);

        auto current =
            toMap(currentResults);


        printRegression(
            baseline,
            current
        );
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Error: "
            << e.what()
            << '\n';

        return 1;
    }


    return 0;
}