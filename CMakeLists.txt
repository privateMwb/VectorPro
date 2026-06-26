cmake_minimum_required(VERSION 3.20)
project(VectorPro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Tests ---
add_executable(tests
    tests/test_all.cpp
    tests/test_constructor.cpp
    tests/test_modifiers.cpp
    tests/test_observer.cpp
    tests/test_element_access.cpp
    tests/test_operators.cpp
    tests/test_capacity.cpp
    tests/test_iterators.cpp
)
target_include_directories(tests PRIVATE include)

# --- Benchmarks ---
add_executable(benchmarks
    benchmarks/benchmark_all.cpp
    benchmarks/benchmark_constructor.cpp
    benchmarks/benchmark_modifiers.cpp
    benchmarks/benchmark_observer.cpp
    benchmarks/benchmark_element_access.cpp
    benchmarks/benchmark_operators.cpp
    benchmarks/benchmark_capacity.cpp
    benchmarks/benchmark_iterators.cpp
)
target_include_directories(benchmarks PRIVATE include)

# --- Examples ---
add_executable(example1 examples/example_1.cpp)
target_include_directories(example1 PRIVATE include)

add_executable(example2 examples/example_2.cpp)
target_include_directories(example2 PRIVATE include)
