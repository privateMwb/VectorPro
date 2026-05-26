#include <iostream>
#include <string>
#include <iterator>

#include "VectorPro.h"

using std::size_t;

struct IntSample {
    int values[5]  = {24, 789, 290, -89, 5000};
    int inserts[3] = {-53, 798, -1};
    int emplace = 777;
};

struct DoubleSample {
    double values[5] = {2.32, -762.438, 89.99, 9000.0003, 1.00};
    double inserts[3] = {0.0002, 1849.91, -67.88};
    double emplace = 983.213;
};

struct StringSample {
    std::string values[5] = {"apple", "banana", "mango", "grapes", "melon"};
    std::string inserts[3] = {"oranges", "tomato", "potato"};
    std::string emplace = "chilli";
};


constexpr IntSample sample;
//constexpr DoubleSample sample;
//constexpr StringSample sample;

template<typename T>
void display_vector(
    VectorPro<T>& old_vp,
    VectorPro<T>& new_vp,
    const std::string& action)
{
    std::cout << "=== "
              << action
              << " ===\n";

    std::cout << "old: ";
    for(const T& x : old_vp) {
        std::cout << x << ' ';
    }

    std::cout << "\nnew: ";
    for(const T& x : new_vp) {
        std::cout << x << ' ';
    }

    std::cout << "\n\n";
}

template<typename T>
void push_back_sample(VectorPro<T>& vp) {

    VectorPro<T> vp_copy(vp);

    for(size_t i = 0;
        i < std::size(sample.values);
        ++i)
    {
        vp.push_back(sample.values[i]);
    }

    display_vector(vp_copy, vp, "PushBack");
}

template<typename T>
void insert_sample(VectorPro<T>& vp) {

    VectorPro<T> vp_copy(vp);

    for(size_t i = 0;
        i < std::size(sample.inserts);
        ++i)
    {
        vp.insert(i, sample.inserts[i]);
    }

    display_vector(vp_copy, vp, "Insert");
}

template<typename T>
void pop_back_sample(VectorPro<T>& vp) {

    VectorPro<T> vp_copy(vp);

    vp.pop_back();

    display_vector(vp_copy, vp, "PopBack");
}

template<typename T>
void erase_sample(VectorPro<T>& vp) {

    VectorPro<T> vp_copy(vp);

    if(!vp.empty()) {
        vp.erase(1);
    }

    display_vector(vp_copy, vp, "Erase");
}

template<typename T>
void remove_if_sample(VectorPro<T>& vp) {

    VectorPro<T> vp_copy(vp);

    vp.remove_if([](const T& x) {
        return x % 2 == 0;
    });

    display_vector(vp_copy, vp, "RemoveIf");
}

template<typename T>
void reserve_sample(VectorPro<T>& vp) {

    std::cout << "=== Reserve ===\n";

    std::cout << "old capacity: "
              << vp.capacity()
              << '\n';

    vp.reserve(50);

    std::cout << "new capacity: "
              << vp.capacity()
              << "\n\n";
}

template<typename T>
void shrink_to_fit_sample(VectorPro<T>& vp) {

    std::cout << "=== ShrinkToFit ===\n";

    std::cout << "old capacity: "
              << vp.capacity()
              << '\n';

    vp.shrink_to_fit();

    std::cout << "new capacity: "
              << vp.capacity()
              << "\n\n";
}

template<typename T>
void clear_sample(VectorPro<T>& vp) {

    VectorPro<T> vp_copy(vp);

    vp.clear();

    display_vector(vp_copy, vp, "Clear");
}

template<typename T>
void emplace_back_sample(VectorPro<T>& vp) {

    VectorPro<T> vp_copy(vp);

    vp.emplace_back(sample.emplace);

    display_vector(vp_copy, vp, "EmplaceBack");
}

template<typename T>
void iterator_sample(VectorPro<T>& vp) {

    std::cout << "=== Iterator ===\n";

    for(auto it = vp.begin();
        it != vp.end();
        ++it)
    {
        std::cout << *it << ' ';
    }

    std::cout << "\n\n";
}

int main() {
    VectorPro<int> vp;
    //VectorPro<double> vp;
    //VectorPro<std::string> vp;

    push_back_sample(vp);

    pop_back_sample(vp);

    insert_sample(vp);

    erase_sample(vp);

    emplace_back_sample(vp);

    iterator_sample(vp);

    reserve_sample(vp);

    //remove_if_sample(vp);

    shrink_to_fit_sample(vp);

    clear_sample(vp);

    return 0;
}