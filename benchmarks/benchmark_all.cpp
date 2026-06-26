#include <iostream>

#include "Vector.h"

void run_constructor_benchmarks();
void run_modifier_benchmarks();
void run_observer_benchmarks();
void run_element_access_benchmarks();
void run_operator_benchmarks();
void run_capacity_benchmarks();
void run_iterator_benchmarks();

int main() {
    run_constructor_benchmarks();
    run_modifier_benchmarks();
    run_observer_benchmarks();
    run_element_access_benchmarks();
    run_operator_benchmarks();
    run_capacity_benchmarks();
    run_iterator_benchmarks();
    return 0;
}
