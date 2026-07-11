#include <common/framework.h>

#include <iomanip>

int main(int argc, char* argv[]) {
    // No args: run every registered suite.
    if (argc == 1) {
        for (const auto& suite : bench_registry()) {
            std::cout << "\n";
            setHeader(suite.name);
            suite.run();
        }
        std::cout << "\n";
        exportJson("benchmark_results.json");
        return 0;
    }

    std::string_view requested = argv[1];

    if (requested == "list") {
        std::cout << "\nAvailable bench suites:\n";
        std::string category;
        for (const auto& suite : bench_registry()) {
            if (category != suite.category)
                std::cout << '\n' << CYAN << prettify(suite.category) << RESET << '\n';
            std::cout << GREEN << std::left << std::setw(6) << ("[" + suite.id + "]") << RESET
                      << std::setw(30) << suite.name << '\n';
            category = suite.category;
        }
        std::cout << "\n";
        return 0;
    }

    const std::string requestedLower = toLower(prettify(requested));
    bool foundCategory = false;

    for (const auto& suite : bench_registry()) {
        const std::string nameLower = toLower(suite.name);
        const std::string idLower = toLower(suite.id);
        const std::string categoryLower = toLower(suite.category);

        if (nameLower == requestedLower || idLower == requestedLower) {
            std::cout << "\n";
            setHeader(suite.name);
            suite.run();
            std::cout << "\n";
            exportJson("benchmark_results.json");
            return 0;
        }

        if (categoryLower == requestedLower) {
            foundCategory = true;
            std::cout << "\n";
            setHeader(suite.name);
            suite.run();
            std::cout << "\n";
        }
    }

    if (foundCategory) {
        exportJson("benchmark_results.json");
        return 0;
    }

    std::cerr << "\nUnknown bench suite: " << requested << "\n\n";
    return 1;
}