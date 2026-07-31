// Vector custom type example.
//
// Demonstrates:
// - Vector storing a non-trivial user-defined type
// - construction, push_back, and copy semantics with a class member
// - sorting with a custom comparator
// - printing struct fields via iteration

#include <support/framework.h>

#include <algorithm>
#include <string>

using namespace VectorPro;

// A small non-trivial type: owns a std::string, so it is not
// trivially copyable and exercises Vector's real copy/move paths.
struct Employee {
    std::string name;
    int level;

    Employee(std::string n, int l) : name(std::move(n)), level(l) {}
};

// Prints the contents of a Vector<Employee>.
static void printEmployees(const Vector<Employee>& v) {
    for (const auto& e : v)
        std::cout << "  " << e.name << " (level " << e.level << ")\n";
}

static void run_examples() {
    // Construct a Vector of a non-trivial type.
    setTitle("Construction");

    Vector<Employee> team;
    team.push_back(Employee{"Alice", 3});
    team.push_back(Employee{"Bob", 1});
    team.emplace_back("Carol", 2);

    std::cout << "Team:\n";
    printEmployees(team);
    std::cout << "\n";

    // Copying a Vector of a non-trivial type deep-copies every element.
    setTitle("Copy Semantics");

    Vector<Employee> teamCopy(team);
    teamCopy[0].level = 99;

    std::cout << "Original after copy mutated:\n";
    printEmployees(team);
    std::cout << "Copy (independent):\n";
    printEmployees(teamCopy);
    std::cout << "\n";

    // Sort with a custom comparator.
    setTitle("Custom Comparator Sort");

    std::sort(team.begin(), team.end(),
              [](const Employee& a, const Employee& b) { return a.level < b.level; });

    std::cout << "Sorted by level:\n";
    printEmployees(team);
}

REGISTER_EXAMPLE_SUITE();
