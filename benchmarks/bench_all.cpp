#include <iostream>

void run_constructor_benchmarks();
void run_modifiers_benchmarks();
void run_capacity_benchmarks();
void run_search_benchmarks();
void run_iteration_benchmarks();
void run_observer_benchmarks();
void run_comparison_benchmarks();

int main() {
    std::cout << "\n";
    
    run_constructor_benchmarks();
    run_modifiers_benchmarks();
    run_capacity_benchmarks();
    run_search_benchmarks();
    run_iteration_benchmarks();
    run_observer_benchmarks();
    run_comparison_benchmarks();
    
    std::cout << "\n";
    return 0;
}
