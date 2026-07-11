# ============================================================
# tests/CMakeLists.txt
# Build configuration for the VectorPro test suite.
# ============================================================

# Collect all test source files.
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Build the test executable.
add_executable(
    tests
    ${TEST_SOURCES}
)

# Allow simple test includes.
target_include_directories(
    tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link VectorPro to the test executable.
target_link_libraries(
    tests
    PRIVATE
    VectorPro
)

add_test(
    NAME VectorProTests
    COMMAND tests
)
