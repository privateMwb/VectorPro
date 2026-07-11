vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/VectorPro
    REF v1.0.0
    SHA512 302d4912e103a96627264a5a6218172c6e662b6d1e4738ac56b76dbae6e2550c5b071b1897fa2e0bb5b2eb3e25426a325a2262545249caa5fef4a142fdca51d4
)

# VectorPro is header-only; consumers only need the library itself,
# not the tests, benchmarks, tools, or examples. tools/ also fetches
# nlohmann_json via FetchContent at configure time, which requires
# network access that vcpkg's build sandbox does not allow.
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DVECTORPRO_BUILD_TESTS=OFF
        -DVECTORPRO_BUILD_BENCHMARKS=OFF
        -DVECTORPRO_BUILD_TOOLS=OFF
        -DVECTORPRO_BUILD_EXAMPLES=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME VectorPro
)

file(
    REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug"
)

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE"
)