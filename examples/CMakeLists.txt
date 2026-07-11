# ============================================================
# examples/CMakeLists.txt
# Build configuration for the VectorPro examples.
# ============================================================

# Collect all example source files.
file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Build the example executable.
add_executable(
    examples
    ${EXAMPLE_SOURCES}
)

# Allow simple example includes.
target_include_directories(
    examples
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link VectorPro to the example executable.
target_link_libraries(
    examples
    PRIVATE
    VectorPro
)
