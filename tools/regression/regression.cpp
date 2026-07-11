#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

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


    std::cout << "\n=== Regression Report ===\n\n";


    for (const auto& [name, currentTime] : current)
    {
        auto it = baseline.find(name);

        if (it == baseline.end())
            continue;


        double oldTime = it->second;


        // Positive means slower
        double change =
            ((currentTime - oldTime) / oldTime) * 100.0;


        std::cout << name << "\n";

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
                << "  FAILURE  : "
                << std::fixed
                << std::setprecision(2)
                << change
                << "% slower\n";
        }
        else if (change > warningThreshold)
        {
            std::cout
                << "  Warning  : "
                << std::fixed
                << std::setprecision(2)
                << change
                << "% slower\n";
        }
        else if (change < -warningThreshold)
        {
            std::cout
                << "  Change   : "
                << std::fixed
                << std::setprecision(2)
                << -change
                << "% faster\n";
        }
        else
        {
            std::cout
                << "  Stable   : "
                << std::fixed
                << std::setprecision(2)
                << change
                << "%\n";
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