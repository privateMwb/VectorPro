#include "test_helper.h"

#include <iostream>

void run_constructor_tests();
void run_modifiers_tests();
void run_observer_tests();
void run_comparison_tests();
void run_access_tests();
void run_search_tests();
void run_capacity_tests();
void run_iterators_tests();
void run_swap_tests();

int main() {
    run_constructor_tests();
    run_modifiers_tests();
    run_observer_tests();
    run_comparison_tests();
    run_access_tests();
    run_search_tests();
    run_capacity_tests();
    run_iterators_tests();
    run_swap_tests();
    
    stats();
    return 0;
}
