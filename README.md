# Grevir Test Support

Development-only CMake support shared by Grevir's native Catch2/CTest tests.
This first increment centralizes dependency setup; the legacy test framework,
Arduino mocks and register/interrupt fixtures have not been extracted. Current
Core and peripheral fixtures live with the tests that use them.

The workspace loads this member when `GREVIR_BUILD_HOST_TESTS=ON`. For a standalone
library checkout, install this helper first:

```sh
cmake -S . -B build/package
cmake --install build/package --prefix /your/grevir-install
```

Then configure the library with `CMAKE_PREFIX_PATH=/your/grevir-install` and
`GREVIR_BUILD_HOST_TESTS=ON`. Its tests call
`find_package(grevir-test-support CONFIG REQUIRED)` to load `GrevirTesting.cmake`.
Native builds are required; cross-compiling host tests is rejected.

Provision an installed Catch2 **3.8.1** package or set
`GREVIR_CATCH2_SOURCE_DIR` to an existing 3.8.1 source checkout for offline setup.
Only explicit `GREVIR_FETCH_TEST_DEPENDENCIES=ON` permits downloading the pinned,
SHA-256-verified archive during configuration. Test execution never downloads
anything. Catch2 and these helpers are absent from production library exports and
public headers. Installing this helper alone does not acquire Catch2.
