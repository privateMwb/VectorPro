# ============================================================
# tools/CMakeLists.txt
# Build configuration for benchmark analysis tools:
#   - compare:    compares VectorPro against std::vector results.
#   - regression: compares benchmark results across versions.
# ============================================================

find_package(nlohmann_json CONFIG REQUIRED)

# ============================================================================
# Compare Tool
# ============================================================================

# Build the compare executable.
add_executable(
    compare
    compare/compare.cpp
)

# Allow simple compare includes.
target_include_directories(
    compare
    PRIVATE
        ${CMAKE_SOURCE_DIR}/benchmarks
)

# Link nlohmann_json to the compare executable.
target_link_libraries(
    compare
    PRIVATE
        nlohmann_json::nlohmann_json
)

# ============================================================================
# Regression Tool
# ============================================================================

# Build the regression executable.
add_executable(
    regression
    regression/regression.cpp
)

# Allow simple regression includes.
target_include_directories(
    regression
    PRIVATE
        ${CMAKE_SOURCE_DIR}/benchmarks
)

# Link nlohmann_json to the regression executable.
target_link_libraries(
    regression
    PRIVATE
        nlohmann_json::nlohmann_json
)