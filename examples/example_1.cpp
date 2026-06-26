#include "Vector.h"
#include "example_helper.h"

#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>

using namespace VectorPro;

int main() {

    // Construction
    setTitle("Construction");

    Vector<int> v;
    std::cout << "Default size: "     << v.size()     << "\n";
    std::cout << "Default capacity: " << v.capacity() << "\n";

    Vector<int> filled(5, 99);
    std::cout << "Fill constructed (5 x 99): ";
    for (const auto& x : filled) std::cout << x << " ";
    std::cout << "\n";

    Vector<int> from_list = { 10, 20, 30, 40, 50 };
    std::cout << "Initializer list: ";
    for (const auto& x : from_list) std::cout << x << " ";
    std::cout << "\n";

    Vector<int> from_range(from_list.begin(), from_list.end());
    std::cout << "Range constructed: ";
    for (const auto& x : from_range) std::cout << x << " ";
    std::cout << "\n\n";

    // Push back & emplace back
    setTitle("Push back & emplace back");

    Vector<std::string> words;
    words.push_back("hello");
    words.push_back("world");
    words.emplace_back(3, '!');
    std::cout << "Words: ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n";
    std::cout << "Size: " << words.size() << "\n\n";

    // Insert
    setTitle("Insert");

    Vector<int> nums = { 1, 2, 4, 5 };
    (void)nums.insert(nums.cbegin() + 2, 3);
    std::cout << "After inserting 3 at index 2: ";
    for (const auto& x : nums) std::cout << x << " ";
    std::cout << "\n";

    Vector<int> extra = { 10, 20 };
    nums.insert(nums.cend(), extra.begin(), extra.end());
    std::cout << "After inserting range {10, 20} at end: ";
    for (const auto& x : nums) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "Size after inserts: " << nums.size() << "\n\n";

    // Emplace
    setTitle("Emplace");

    Vector<std::string> sv = { "apple", "cherry" };
    (void)sv.emplace(sv.cbegin() + 1, "banana");
    std::cout << "After emplacing banana at index 1: ";
    for (const auto& w : sv) std::cout << w << " ";
    std::cout << "\n";
    std::cout << "Size: " << sv.size() << "\n\n";

    // Pop back & erase
    setTitle("Pop back & erase");

    Vector<int> e = { 1, 2, 3, 4, 5 };
    e.pop_back();
    std::cout << "After pop_back: ";
    for (const auto& x : e) std::cout << x << " ";
    std::cout << "\n";

    (void)e.erase(e.cbegin() + 1);
    std::cout << "After erasing index 1: ";
    for (const auto& x : e) std::cout << x << " ";
    std::cout << "\n";

    (void)e.erase(e.cbegin(), e.cbegin() + 2);
    std::cout << "After erasing range [0, 2): ";
    for (const auto& x : e) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "Final size: " << e.size() << "\n\n";

    // Element access
    setTitle("Element access");

    Vector<int> acc = { 100, 200, 300, 400, 500 };
    std::cout << "operator[2]:  " << acc[2]           << "\n";
    std::cout << "at(3):        " << acc.at(3)        << "\n";
    std::cout << "front():      " << acc.front()      << "\n";
    std::cout << "back():       " << acc.back()       << "\n";
    std::cout << "data_ptr()[1]:" << acc.data_ptr()[1] << "\n";

    try {
        (void)acc.at(99);
    } catch (const std::out_of_range& ex) {
        std::cout << "at(99) threw: " << ex.what() << "\n";
    }
    std::cout << "\n";

    // Capacity
    setTitle("Capacity");

    Vector<int> cap;
    cap.reserve(20);
    std::cout << "Size after reserve(20):     " << cap.size()     << "\n";
    std::cout << "Capacity after reserve(20): " << cap.capacity() << "\n";

    for (int i = 0; i < 5; ++i) cap.push_back(i);
    cap.shrink_to_fit();
    std::cout << "Size after shrink_to_fit:     " << cap.size()     << "\n";
    std::cout << "Capacity after shrink_to_fit: " << cap.capacity() << "\n\n";

    // Copy & move
    setTitle("Copy & move");

    Vector<int> original = { 1, 2, 3 };
    Vector<int> copy(original);
    copy[0] = 99;
    std::cout << "Original[0] after modifying copy: " << original[0] << "\n";
    std::cout << "Copy[0]: "                          << copy[0]     << "\n";

    Vector<int> moved(std::move(original));
    std::cout << "Moved size: "              << moved.size()    << "\n";
    std::cout << "Original size after move: " << original.size() << "\n\n";

    // Clear
    setTitle("Clear");

    Vector<int> cl = { 1, 2, 3, 4, 5 };
    std::cout << "Size before clear: " << cl.size() << "\n";
    cl.clear();
    std::cout << "Size after clear: "     << cl.size()     << "\n";
    std::cout << "Capacity after clear: " << cl.capacity() << "\n\n";

    // Iterators
    setTitle("Iterators");

    Vector<int> it_vec = { 5, 3, 1, 4, 2 };
    std::sort(it_vec.begin(), it_vec.end());
    std::cout << "After std::sort: ";
    for (const auto& x : it_vec) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Reverse order:   ";
    for (auto it = it_vec.rbegin(); it != it_vec.rend(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";

    int sum = std::accumulate(it_vec.begin(), it_vec.end(), 0);
    std::cout << "std::accumulate sum: " << sum << "\n\n";

    return 0;
}