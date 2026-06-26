#include "Vector.h"
#include "example_helper.h"

#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>
#include <memory_resource>

using namespace VectorPro;

int main() {

    // Observer system
    setTitle("Observer system");

    Vector<int> v;
    int eventCount = 0;

    auto handle = v.subscribe([&eventCount](const Vector<int>&, Vector<int>::EventData data) {
        ++eventCount;
        std::cout << "Event fired — type: " << static_cast<int>(data.type)
                  << " oldSize: " << data.oldSize
                  << " newSize: " << data.newSize << "\n";
    });

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.pop_back();
    std::cout << "Total events fired: " << eventCount << "\n";

    v.unsubscribe(handle);
    v.push_back(99);
    std::cout << "After unsubscribe, events fired: " << eventCount << "\n\n";

    // Multiple listeners
    setTitle("Multiple listeners");

    Vector<int> mv;
    int countA = 0;
    int countB = 0;

    (void)mv.subscribe([&countA](const Vector<int>&, Vector<int>::EventData) { ++countA; });
    (void)mv.subscribe([&countB](const Vector<int>&, Vector<int>::EventData) { ++countB; });

    mv.push_back(1);
    mv.push_back(2);
    mv.push_back(3);
    std::cout << "Listener A fired: " << countA << " times\n";
    std::cout << "Listener B fired: " << countB << " times\n\n";

    // remove_if
    setTitle("Remove if");

    Vector<int> r = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::cout << "Before remove_if: ";
    for (const auto& x : r) std::cout << x << " ";
    std::cout << "\n";

    std::size_t removed = r.remove_if([](const int& x) { return x % 2 == 0; });
    std::cout << "After removing evens: ";
    for (const auto& x : r) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "Removed count: " << removed << "\n\n";

    // Search
    setTitle("Search");

    Vector<int> s = { 10, 20, 30, 40, 50 };
    std::cout << "contains(30): " << (s.contains(30) ? "true" : "false") << "\n";
    std::cout << "contains(99): " << (s.contains(99) ? "true" : "false") << "\n";

    auto it = s.find(40);
    if (it != s.end())
        std::cout << "find(40): found " << *it << "\n";

    auto missing = s.find(99);
    std::cout << "find(99): " << (missing == s.end() ? "not found" : "found") << "\n\n";

    // Span accessor
    setTitle("Span accessor");

    Vector<int> sp = { 1, 2, 3, 4, 5 };
    auto span = sp.as_span();
    std::cout << "as_span size: " << span.size() << "\n";
    std::cout << "Span elements: ";
    for (const auto& x : span) std::cout << x << " ";
    std::cout << "\n";

    int spanSum = std::accumulate(span.begin(), span.end(), 0);
    std::cout << "Span accumulate: " << spanSum << "\n\n";

    // Comparison operators
    setTitle("Comparison operators");

    Vector<int> a = { 1, 2, 3 };
    Vector<int> b = { 1, 2, 3 };
    Vector<int> c = { 1, 2, 4 };

    std::cout << "a == b: " << (a == b ? "true" : "false") << "\n";
    std::cout << "a == c: " << (a == c ? "true" : "false") << "\n";
    std::cout << "a < c:  " << (a <  c ? "true" : "false") << "\n";
    std::cout << "c > a:  " << (c >  a ? "true" : "false") << "\n";
    std::cout << "a <= b: " << (a <= b ? "true" : "false") << "\n";
    std::cout << "a >= b: " << (a >= b ? "true" : "false") << "\n\n";

    // Swap
    setTitle("Swap");

    Vector<int> x = { 1, 2, 3 };
    Vector<int> y = { 4, 5, 6 };
    std::cout << "Before swap - x: ";
    for (const auto& n : x) std::cout << n << " ";
    std::cout << "\n";
    std::cout << "Before swap - y: ";
    for (const auto& n : y) std::cout << n << " ";
    std::cout << "\n";

    x.swap(y);
    std::cout << "After swap  - x: ";
    for (const auto& n : x) std::cout << n << " ";
    std::cout << "\n";
    std::cout << "After swap  - y: ";
    for (const auto& n : y) std::cout << n << " ";
    std::cout << "\n\n";

    // Custom growth factor
    setTitle("Custom growth factor");

    Vector<int, std::allocator<int>, 3, 2> gv;
    std::cout << "Growth factor: 1.5x (3/2)\n";
    for (int i = 0; i < 10; ++i)
        gv.push_back(i);
    std::cout << "Size:     " << gv.size()     << "\n";
    std::cout << "Capacity: " << gv.capacity() << "\n\n";

    // Custom allocator
    setTitle("Custom allocator");

    std::pmr::monotonic_buffer_resource pool;
    Vector<int, std::pmr::polymorphic_allocator<int>> pv{
        std::pmr::polymorphic_allocator<int>(&pool)
    };

    for (int i = 0; i < 5; ++i)
        pv.push_back(i * 10);

    std::cout << "PMR allocated vector: ";
    for (const auto& x : pv) std::cout << x << " ";
    std::cout << "\n";
    std::cout << "Size:     " << pv.size()     << "\n";
    std::cout << "Capacity: " << pv.capacity() << "\n\n";

    return 0;
}