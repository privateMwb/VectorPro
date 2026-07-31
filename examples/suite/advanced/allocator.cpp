// Vector allocator example.
//
// Demonstrates:
// - Vector with std::pmr::polymorphic_allocator
// - Vector with a custom stateful allocator
// - Allocator propagation on copy

#include <support/framework.h>

#include <memory>
#include <memory_resource>

using namespace VectorPro;

// A minimal stateful allocator that counts allocate/deallocate calls.
// Demonstrates that Vector works with allocators beyond std::allocator.
template <typename T> struct CountingAllocator {
    using value_type = T;

    std::size_t* allocCount = nullptr;
    std::size_t* deallocCount = nullptr;

    CountingAllocator() = default;
    CountingAllocator(std::size_t& allocs, std::size_t& deallocs)
        : allocCount(&allocs), deallocCount(&deallocs) {}

    template <typename U>
    CountingAllocator(const CountingAllocator<U>& other) noexcept
        : allocCount(other.allocCount), deallocCount(other.deallocCount) {}

    T* allocate(std::size_t n) {
        if (allocCount)
            ++(*allocCount);
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T* p, std::size_t n) noexcept {
        if (deallocCount)
            ++(*deallocCount);
        std::allocator<T>{}.deallocate(p, n);
    }

    template <typename U> bool operator==(const CountingAllocator<U>&) const noexcept {
        return true;
    }
};

static void run_examples() {
    // std::pmr::polymorphic_allocator with a monotonic buffer resource.
    setTitle("PMR Allocator");

    std::byte buffer[4096];
    std::pmr::monotonic_buffer_resource resource(buffer, sizeof(buffer));
    std::pmr::polymorphic_allocator<int> pmrAlloc(&resource);

    Vector<int, std::pmr::polymorphic_allocator<int>> pv(pmrAlloc);
    pv.push_back(1);
    pv.push_back(2);
    pv.push_back(3);

    std::cout << "Size     : " << pv.size() << "\n";
    std::cout << "Elements : ";
    for (int x : pv)
        std::cout << x << " ";
    std::cout << "\n\n";

    // Custom stateful allocator tracking allocation counts.
    setTitle("Custom Counting Allocator");

    std::size_t allocs = 0, deallocs = 0;
    CountingAllocator<int> counting(allocs, deallocs);

    Vector<int, CountingAllocator<int>> cv(counting);
    for (int i = 0; i < 20; ++i)
        cv.push_back(i);

    std::cout << "Size             : " << cv.size() << "\n";
    std::cout << "Allocate calls   : " << allocs << "\n";
    std::cout << "Deallocate calls : " << deallocs << "\n\n";

    // Demonstrates allocator behavior during copy construction.
    setTitle("Allocator Propagation");

    Vector<int, CountingAllocator<int>> copy(cv);
    std::cout << "Copy size : " << copy.size() << "\n";
    std::cout << "Copy shares allocator type, independent allocation count\n";
}

REGISTER_EXAMPLE_SUITE();