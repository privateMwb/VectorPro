// clang-format off
#include <common/framework.h>   // test_registry(), setTitle(), stats(), prettify(),
                                // toLower(), color constants

#include <iomanip>              // std::setw
// clang-format on

int main(int argc, char* argv[]) {
    // No args: run every registered suite.
    if (argc == 1) {
        for (const auto& suite : test_registry()) {
            std::cout << "\n";
            setTitle(suite.name);
            suite.run();
        }
        std::cout << "\n";
        stats();
        std::cout << "\n";
        return 0;
    }

    std::string_view requested = argv[1];

    // "list": print every registered suite, grouped by category, no run.
    if (requested == "list") {
        std::cout << "\nAvailable test suites:\n";
        std::string category;
        for (const auto& suite : test_registry()) {
            if (category != suite.category)
                std::cout << '\n' << CYAN << prettify(suite.category) << RESET << '\n';
            std::cout << GREEN << std::left << std::setw(6) << ("[" + suite.id + "]") << RESET
                      << std::setw(30) << suite.name << '\n';
            category = suite.category;
        }
        std::cout << "\n";
        return 0;
    }

    // Otherwise: run whichever suite(s) match the requested name, id, or category.
    const std::string requestedLower = toLower(prettify(requested));
    bool foundCategory = false;

    for (const auto& suite : test_registry()) {
        const std::string nameLower = toLower(suite.name);
        const std::string idLower = toLower(suite.id);
        const std::string categoryLower = toLower(suite.category);

        // Exact suite match (by name or id): run just this one and exit.
        if (nameLower == requestedLower || idLower == requestedLower) {
            std::cout << "\n";
            setTitle(suite.name);
            suite.run();
            std::cout << "\n";
            stats();
            std::cout << "\n";
            return 0;
        }

        // Category match: run every suite in it, keep scanning for more.
        if (categoryLower == requestedLower) {
            foundCategory = true;
            std::cout << "\n";
            setTitle(suite.name);
            suite.run();
            std::cout << "\n";
        }
    }

    if (foundCategory) {
        stats();
        return 0;
    }

    std::cerr << "\nUnknown test suite: " << requested << "\n\n";
    return 1;
}
