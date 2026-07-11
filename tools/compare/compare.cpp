#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Result {
    std::string library;
    std::string operation;
    double ns_per_op;
};

std::vector<Result> loadResults(const std::string& file)
{
    std::ifstream in(file);

    if (!in)
        throw std::runtime_error("Cannot open: " + file);

    json data;
    in >> data;

    std::vector<Result> results;

    for (const auto& item : data)
    {
        results.push_back({
            item["library"],
            item["operation"],
            item["ns_per_op"]
        });
    }

    return results;
}

int main()
{
    auto results = loadResults("benchmark_results.json");

    std::unordered_map<std::string, double> vectorPro;
    std::unordered_map<std::string, double> stdVector;

    for (const auto& r : results)
    {
        if (r.library == "VectorPro")
            vectorPro[r.operation] = r.ns_per_op;

        else if (r.library == "std::vector")
            stdVector[r.operation] = r.ns_per_op;
    }


    std::cout << "\n=== VectorPro vs std::vector ===\n\n";


    for (const auto& [operation, vpTime] : vectorPro)
    {
        auto it = stdVector.find(operation);

        if (it == stdVector.end())
            continue;


        double stdTime = it->second;

        double percent =
            ((stdTime - vpTime) / stdTime) * 100.0;


        std::cout << operation << "\n";

        std::cout
            << "  VectorPro     : "
            << vpTime
            << " ns/op\n";

        std::cout
            << "  std::vector   : "
            << stdTime
            << " ns/op\n";


        if (percent > 0)
        {
            std::cout
                << "  Advantage     : "
                << percent
                << "% faster\n";
        }
        else
        {
            std::cout
                << "  Difference    : "
                << -percent
                << "% slower\n";
        }

        std::cout << "\n";
    }

    return 0;
}