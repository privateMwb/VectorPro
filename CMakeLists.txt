# ============================================================
# CMakeLists.txt
# Build configuration for the VectorPro library.
# ============================================================
#
# Sections:
#   1. C++ Configuration
#   2. VectorPro Library
#   3. Subdirectories
#   4. Installation
#   5. Packaging
#
# ============================================================

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

# Place all executables directly in the build directory, regardless of
# which subdirectory defines them (e.g. build/tests, build/compare).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ============================================================================
# VectorPro (Header-only Library)
# ============================================================================

# Define the header-only VectorPro library.
add_library(VectorPro INTERFACE)

# Expose the public include directory to consumers.
target_include_directories(
    VectorPro
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Require C++23 for all targets that consume VectorPro.
target_compile_features(
    VectorPro
    INTERFACE
    cxx_std_23
)

# ============================================================================
# Subdirectories
# ============================================================================

enable_testing()

add_subdirectory(tests tests_build)
add_subdirectory(benchmarks benchmarks_build)
add_subdirectory(tools tools_build)
add_subdirectory(examples examples_build)

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
