// VectorPro Examples
// Demonstrates the core features of VectorPro:
//
// - constructors
// - push_back
// - pop_back
// - insert
// - erase
// - emplace_back
// - remove_if
// - shrink_to_fit
// - reserve
// - clear
// - notify

#include <iostream>
#include <cstddef>

#include "VectorPro.h"

// Constructors Example
// demonstrates the different ways to construct a VectorPro
void constructors() {
    // Default construction
    VectorPro<int> vp1;
    VectorPro<std::string> vp2;
    VectorPro<double> vp3;
    VectorPro<bool> vp4;

    // Dynamic construction
    VectorPro<int>* vp5 = new VectorPro<int>();

    // Initializer list construction
    VectorPro<int> vp6{1, 2, 3, 4, 5};

    // Copy construction
    VectorPro<int> vp7(vp6);

    // Move construction
    VectorPro<int> vp8(std::move(vp7));

    delete vp5;
}

// Push Back Example
// appends elements to the end of the container
void push_back() {
    // Integer values
    VectorPro<int> vp1;
    vp1.push_back(10);
    vp1.push_back(20);
    vp1.push_back(30);

    // String values
    VectorPro<std::string> vp2;
    vp2.push_back("apple");
    vp2.push_back("banana");
    vp2.push_back("orange");

    // Boolean values
    VectorPro<bool> vp3;
    vp3.push_back(true);
    vp3.push_back(false);

    // Floating-point values
    VectorPro<double> vp4;
    vp4.push_back(3.14);
    vp4.push_back(2.718);
}

// Pop Back Example
// removes the last element from the container
void pop_back() {
    VectorPro<int> vp = {2, 4, 6, 8, 10};

    vp.pop_back(); // {2, 4, 6, 8}
}

// Insert Example
// inserts an element at the specified index
void insert() {
    VectorPro<int> vp = {1, 3, 4, 5};

    vp.insert(1, 2); // insert value 2 at index 1 -> {1, 2, 3, 4, 5}
}

// Erase Example
// removes the element at the specified index
void erase() {
    VectorPro<int> vp = {1, 2, 3, 4, 5};

    vp.erase(3); // erase index 3 -> {1, 2, 3, 5}
}

// Emplace Back Example
// constructs an element directly at the end of the container
void emplace_back() {
    struct Person {
        std::string name;
        int age;

        Person(std::string n, int a)
            : name(std::move(n)), age(a) {}
    };

    VectorPro<Person> vp;

    vp.emplace_back("Alice", 20);
    vp.emplace_back("Bob", 25);
}

// Remove If Example
// removes all elements that satisfy a predicate
void remove_if() {
    VectorPro<int> vp = {21, 103, 0, 89, 70};

    vp.remove_if([](int i) {
        return i % 2 == 0;
    }); // remove even numbers -> {21, 103, 89}
}

// Shrink To Fit Example
// reduces capacity to match the current size
void shrink_to_fit() {
    VectorPro<int> vp;

    vp.reserve(100);

    vp.push_back(40);
    vp.push_back(0);
    vp.push_back(592);
    vp.push_back(100);
    vp.push_back(33);

    vp.shrink_to_fit(); // capacity becomes equal to size
}

// Reserve Example
// reserves storage for future elements
void reserve() {
    VectorPro<int> vp;

    vp.reserve(100); // reserve capacity for at least 100 elements
}

// Clear Example
// removes all elements from the container
void clear() {
    VectorPro<int> vp = {10, 20, 30, 40, 50};

    vp.clear(); 
}

// Notify Example
// shows event-driven updates from VectorPro
void notify() {
    VectorPro<int> vp;

    vp.subscribe([](const VectorPro<int>& v, EventType event) {
        switch (event) {
            case EventType::PushBack:
                std::cout << "PushBack triggered\n";
                break;
            case EventType::PopBack:
                std::cout << "PopBack triggered\n";
                break;
            case EventType::Clear:
                std::cout << "Clear triggered\n";
                break;
            default:
                std::cout << "Other event triggered\n";
        }

        std::cout << "Current size: " << v.size() << '\n';
    });

    vp.push_back(1);
    vp.push_back(2);
    vp.pop_back();
    vp.clear();
}

int main() {
    constructors();
    push_back();
    pop_back();
    insert();
    erase();
    emplace_back();
    remove_if();
    shrink_to_fit();
    reserve();
    clear();
    notify();
    
    return 0;
}