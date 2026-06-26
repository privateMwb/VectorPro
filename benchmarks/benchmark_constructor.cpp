// Vector Constructor Benchmark
// Measures construction and assignment performance
// against std::vector where applicable.
//
// Covers:
// - Default construction
// - Fill construction
// - Initializer list construction
// - Copy construction
// - Move construction
// - Copy assignment
// - Move assignment
// - Custom growth factor construction
// - Custom allocator construction

#include "benchmark_helper.h"

#include <vector>
#include <memory_resource>

using namespace VectorPro;

static void default_construct() {
    BENCH("VectorPro default construct",   ITERATIONS, Vector<int>{});
    BENCH("std::vector default construct", ITERATIONS, std::vector<int>{});
}

static void fill_construct() {
    BENCH("VectorPro fill construct",   ITERATIONS, Vector<int>(100, 0));
    BENCH("std::vector fill construct", ITERATIONS, std::vector<int>(100, 0));
}

static void init_list_construct() {
    BENCH("VectorPro init list construct",   ITERATIONS, (Vector<int>{ 1, 2, 3, 4, 5 }));
    BENCH("std::vector init list construct", ITERATIONS, (std::vector<int>{ 1, 2, 3, 4, 5 }));
}

static void copy_construct() {
    Vector<int>      src;
    std::vector<int> stdSrc;
    fillVector(src, 1000);
    fillStd(stdSrc, 1000);

    BENCH("VectorPro copy construct",   ITERATIONS, (void)Vector<int>(src));
    BENCH("std::vector copy construct", ITERATIONS, (void)std::vector<int>(stdSrc));
}

static void move_construct() {
    BENCH("VectorPro move construct", ITERATIONS, [](){
        Vector<int> src;
        fillVector(src, 100);
        Vector<int> v(std::move(src));
        (void)v;
    }());

    BENCH("std::vector move construct", ITERATIONS, [](){
        std::vector<int> src;
        fillStd(src, 100);
        std::vector<int> v(std::move(src));
        (void)v;
    }());
}

static void copy_assignment() {
    Vector<int>      src;
    std::vector<int> stdSrc;
    fillVector(src, 1000);
    fillStd(stdSrc, 1000);

    Vector<int>      dst;
    std::vector<int> stdDst;

    BENCH("VectorPro copy assignment",   ITERATIONS, (void)(dst = src));
    BENCH("std::vector copy assignment", ITERATIONS, (void)(stdDst = stdSrc));
}

static void move_assignment() {
    BENCH("VectorPro move assignment", ITERATIONS, [](){
        Vector<int> src;
        fillVector(src, 100);
        Vector<int> dst;
        dst = std::move(src);
    }());

    BENCH("std::vector move assignment", ITERATIONS, [](){
        std::vector<int> src;
        fillStd(src, 100);
        std::vector<int> dst;
        dst = std::move(src);
    }());
}

static void custom_growth() {
    auto fn = [](){
        Vector<int, std::allocator<int>, 3, 2> v;
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
    };
    BENCH("VectorPro 1.5x growth construct", ITERATIONS, fn());
}

static void custom_allocator() {
    auto fn = [](){
        std::pmr::monotonic_buffer_resource pool;
        Vector<int, std::pmr::polymorphic_allocator<int>> v{
            std::pmr::polymorphic_allocator<int>(&pool)
        };
        for (int i = 0; i < 100; ++i)
            v.push_back(i);
    };
    BENCH("VectorPro PMR allocator construct", ITERATIONS, fn());
}

void run_constructor_benchmarks() {
    bench_header("Constructor");

    default_construct();
    std::cout << "\n";
    
    fill_construct();
    std::cout << "\n";
    
    init_list_construct();
    std::cout << "\n";
    
    copy_construct();
    std::cout << "\n";
    
    move_construct();
    std::cout << "\n";
    
    copy_assignment();
    std::cout << "\n";
    
    move_assignment();
    std::cout << "\n";
    
    custom_growth();
    std::cout << "\n";
    
    custom_allocator();
    bench_footer();
}