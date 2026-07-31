vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/VectorPro
    REF v1.0.0
    SHA512 8b0872b7c10abaf9aef92887f8c7fe4159e69bfe7a6b711a8bfc41b0d64422f1f9759917579ff1ca93c741f5e876bf4bab6193a359bcc92aed83503110f205bf
)

set(VCPKG_PORT_NAME VectorPro)

# Consumers only need the library itself, not the tests, benchmarks,
# regression tools, or examples. regression/ also fetches a third-party
# dependency via FetchContent at configure time, which requires network
# access that vcpkg's build sandbox does not allow.
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_TESTS=OFF
        -DBUILD_BENCHMARKS=OFF
        -DBUILD_REGRESSION=OFF
        -DBUILD_EXAMPLES=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME ${VCPKG_PORT_NAME}
    CONFIG_PATH lib/cmake/${VCPKG_PORT_NAME}
)

# This library is compiled (not header-only), so debug binaries are
# real and must be kept — only the duplicate debug/include headers
# are removed.
file(
    REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
)

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE"
)