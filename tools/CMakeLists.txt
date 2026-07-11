# ============================================================
# tools/CMakeLists.txt
# Build configuration for benchmark analysis tools:
#   - compare:    compares VectorPro against std::vector results.
#   - regression: compares benchmark results across versions.
# ============================================================

# Fetch nlohmann_json at configure time instead of relying on a
# system package, so the build is reproducible across Termux,
# MSYS2, and CI without any OS-specific install step.
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

# ============================================================================
# Compare Tool
# ============================================================================

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
