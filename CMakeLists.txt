cmake_minimum_required(VERSION 3.20)

project(
    VectorPro
    VERSION 1.0.0
    LANGUAGES CXX
)


# ============================================================================
# C++ Configuration
# ============================================================================

# Configure the C++ language standard.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ============================================================================
# VectorPro (Header-only Library)
# ============================================================================

# Define the header-only StackPro library.
add_library(VectorPro INTERFACE)

# Expose the public include directory to consumers.
target_include_directories(
    VectorPro
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Require C++23 for all targets that consume StackPro.
target_compile_features(
    VectorPro
    INTERFACE
    cxx_std_23
)

# ============================================================================
# Tests
# ============================================================================

# Build the test executable.
add_executable(
    tests
    tests/test_all.cpp
    tests/test_constructor.cpp
    tests/test_modifiers.cpp 
    tests/test_observer.cpp
    tests/test_comparison.cpp
    tests/test_access.cpp
    tests/test_search.cpp
    tests/test_capacity.cpp
    tests/test_iterators.cpp
    tests/test_swap.cpp
)

# Link StackPro to the test executable.
target_link_libraries(
    tests
    PRIVATE
    VectorPro
)

# ============================================================================
# Benchmarks
# ============================================================================

# Build the benchmark executable.
add_executable(
    benchmarks 
    benchmarks/bench_all.cpp
    benchmarks/bench_constructor.cpp
    benchmarks/bench_modifiers.cpp
    benchmarks/bench_capacity.cpp
    benchmarks/bench_search.cpp
    benchmarks/bench_iterators.cpp
    benchmarks/bench_observer.cpp
    benchmarks/bench_comparison.cpp
)

# Link StackPro to the benchmark executable.
target_link_libraries(
    benchmarks 
    PRIVATE
    VectorPro
)


# ============================================================================
# Examples
# ============================================================================

# Build every example in the examples/ directory.
file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp"
)

foreach(example ${EXAMPLE_SOURCES})
    get_filename_component(target "${example}" NAME_WE)

    add_executable(${target} "${example}")

    target_link_libraries(
        ${target}
        PRIVATE
        VectorPro
    )
endforeach()


# ============================================================================
# Installation
# ============================================================================

# Install the VectorPro target and export it for downstream projects.
install(
    TARGETS VectorPro
    EXPORT VectorProTargets
)

# Install the exported CMake targets.
install(
    EXPORT VectorProTargets
    FILE VectorProTargets.cmake
    NAMESPACE VectorPro::
    DESTINATION lib/cmake/VectorPro
)

# Install the public header files.
install(
    DIRECTORY include/
    DESTINATION include
)


# ============================================================================
# Packaging
# ============================================================================

include(CMakePackageConfigHelpers)

# Generate the package configuration file.
configure_package_config_file(
    cmake/VectorProConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/VectorProConfig.cmake
    INSTALL_DESTINATION lib/cmake/VectorPro
)

# Generate the package version file.
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/VectorProConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install the package configuration files.
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/VectorProConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/VectorProConfigVersion.cmake
    DESTINATION lib/cmake/VectorPro
)