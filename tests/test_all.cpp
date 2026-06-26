#include <iostream>

#include "test_helper.h"
#include "Vector.h"

void run_constructor_tests();
void run_modifier_tests();
void run_observer_tests();
void run_element_access_tests();
void run_operator_tests();
void run_capacity_tests();
void run_iterator_tests();

int main() {
    run_constructor_tests();
    run_modifier_tests();
    run_observer_tests();
    run_element_access_tests();
    run_operator_tests();
    run_capacity_tests();
    run_iterator_tests();
    
    stats();
    return 0;
}
