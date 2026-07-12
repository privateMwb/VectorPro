# Packaging: Local Verification

Notes for re-verifying the vcpkg port and Conan recipe after changes
(e.g. a new release, editing `portfile.cmake`, or editing
`recipes/vectorpro/all/conanfile.py`). Scoped to what isn't obvious
from the files themselves — not a general build tutorial.

## Termux-specific quirks

Both package managers assume a "normal" Linux/CI environment.
Termux's Android-flavored profile detection breaks several of their
default assumptions. None of this applies on real CI (GitHub Actions
Ubuntu runner, a real Linux box, etc.) — it's purely a Termux problem.

- **`vcpkg`** — not installed on Termux; bootstrapping it needs a full
  C++ toolchain build and has known portability issues on bionic libc.
  Skip it. `sha512sum` produces the identical hash `vcpkg hash` would,
  and a plain `cmake --install` reproduces what `vcpkg_cmake_configure`
  + `vcpkg_cmake_install` do under the hood — that's enough to verify
  the port without the binary.

- **`conan profile detect`** auto-detects `os=Android`, which makes
  `CMakeToolchain` demand `tools.android:ndk_path` — Termux isn't
  actually cross-compiling, so override: `-s os=Linux`.

- **`compiler.libcxx`** auto-detects as `libstdc++11`, but Termux's
  Clang actually ships `libc++`. Wrong value forces `-stdlib=libstdc++`,
  which doesn't exist here and breaks every standard header include.
  Override: `-s compiler.libcxx=libc++`.

- **Host/build profile mismatch** — overriding `os` on the host profile
  only (`-s os=Linux`) while the build profile still says `os=Android`
  confuses Conan into partial cross-compile behavior (breaks include
  path resolution even for basic headers like `<cassert>`). If sticking
  with `CMakeToolchain`, override both: add `-s:b os=Linux` too.

- **Simpler alternative** — if `CMakeToolchain` keeps fighting the
  Android detection, drop it entirely. Use `[generators]` with just
  `CMakeDeps` in the consumer `conanfile.txt`, then build with a plain
  `cmake -B build -DCMAKE_PREFIX_PATH=$(pwd)` instead of the generated
  preset. Trade-off: `CMakeToolchain` normally sets `CMAKE_CXX_STANDARD`
  automatically from `compiler.cppstd`; without it, the consumer's own
  `CMakeLists.txt` must set the C++ standard explicitly, or every
  C++23 concept/`contiguous_iterator_tag` construct in the headers
  fails to compile.

- **`CMAKE_BUILD_TYPE` matters even without a toolchain file** —
  Conan's `CMakeDeps`-generated `INTERFACE_INCLUDE_DIRECTORIES` are
  gated behind `$<$<CONFIG:Release>:...>` generator expressions. If
  `CMAKE_BUILD_TYPE` isn't set to match (e.g. left empty), the include
  directories silently evaluate to nothing — no error, just a
  "file not found" on every header. Always pass
  `-DCMAKE_BUILD_TYPE=Release` explicitly when testing.

## Verifying the vcpkg port

1. Extract the release tarball and install to a staging prefix with
   the same flags `portfile.cmake` uses:
   ```bash
   tar xzf v<version>.tar.gz && cd VectorPro-<version>
   cmake -B build \
     -DCMAKE_INSTALL_PREFIX=$HOME/staging \
     -DVECTORPRO_BUILD_TESTS=OFF \
     -DVECTORPRO_BUILD_BENCHMARKS=OFF \
     -DVECTORPRO_BUILD_TOOLS=OFF \
     -DVECTORPRO_BUILD_EXAMPLES=OFF
   cmake --build build
   cmake --install build
   ```
2. Confirm the CMake package files exist:
   ```bash
   find $HOME/staging -name "*.cmake"
   ```
   Expect `VectorProConfig.cmake`, `VectorProConfigVersion.cmake`,
   `VectorProTargets.cmake` under `lib/cmake/VectorPro/`.
3. In a **separate** directory, with a minimal consumer project
   (`find_package(VectorPro CONFIG REQUIRED)` +
   `target_link_libraries(app PRIVATE VectorPro::VectorPro)`):
   ```bash
   cmake -B build -DCMAKE_PREFIX_PATH=$HOME/staging
   cmake --build build
   ./build/app
   ```

## Verifying the Conan recipe

1. From repo root:
   ```bash
   conan create recipes/vectorpro/all --version <version> \
     -s compiler.cppstd=23 -s os=Linux
   ```
   Watch for: `validate()` passing (C++23 floor check), `LICENSE`
   landing in `package(): Packaged ... LICENSE`, and a single
   `package_id` regardless of settings (confirms `self.info.clear()`
   is working for the header-only package).
2. In a **separate** directory, with `conanfile.txt`:
   ```
   [requires]
   vectorpro/<version>

   [generators]
   CMakeDeps
   ```
   plus the same minimal consumer `CMakeLists.txt`/`main.cpp` as above:
   ```bash
   conan install . -s compiler.cppstd=23 --build=missing
   cmake -B build -DCMAKE_PREFIX_PATH=$(pwd) -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ./build/app
   ```

Both should print `1 2 3 4` (or whatever the consumer's test program
outputs) — that confirms the package is actually consumable, not just
that it builds.
