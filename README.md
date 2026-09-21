# Grevir Test Support

Development-only native test setup and reusable register-memory fixtures. Core
and peripheral application fixtures still live with their owning tests. This
package is never a production dependency of Grevir Registers or AVR.

## Shared register fixture

Include `<grevir/test/register_memory.hpp>` and link `grevir::test_support`.
`grevir::test::RegisterMemory<Capacity, Identity>` provides static `bytes`, `events`,
`reset(fill)`, and typed `read`, `write`, `modify` operations. Different capacity or
identity types have separate storage. Event records contain kind, address, width
and value; barrier-entry/exit kinds are available for caller-owned policies.

Reads/writes use `memcpy`, preserving native object representation without host
alignment assumptions. Unsigned integer accesses up to 64 bits are supported.
Bounds errors throw before memory or traces change. Masked updates restrict their
input value to the mask. Reset fills storage and clears events. This is an ordinary
memory fixture; it does not model electrical state, interrupts, device-side flags,
wire byte order or hardware synchronization. AVR tests currently exercise byte
GPIO operations and use paired native reads/writes for width checks.

The same header adapts legacy `setl::DebugMcuRegister<T, Address, Size, Memory>`.
It retains caller-owned storage and get/set/set_mask, uses `memcpy` in place of the
packed-object pointer, and rejects negative or overflowing regions at compile time.
The old raw `ptr()`/`Wrapper` implementation interface is not retained. This
compatibility accessor does not emit events; use `RegisterMemory` for tracing.

Three runtime cases check reset/isolation, bounds/no-access-on-error, masked
updates and unaligned compatibility access. Compiler probes accept an end-aligned
region and reject negative, crossing-end and oversized regions. Registers and AVR
both pass their suites using the shared header. A fixture-only installed consumer
builds and runs with Catch2 discovery disabled.

## Install and host runner setup

```sh
cmake -S . -B build/package
cmake --install build/package --prefix /your/grevir-install
```

Fixture-only consumers call `find_package(grevir-test-support CONFIG REQUIRED)`
and link `grevir::test_support`; Catch2 is not loaded. With
`GREVIR_BUILD_HOST_TESTS=ON`, package discovery also loads `GrevirTesting.cmake`.
The workspace uses that option for native host tests. Cross-compiling host tests
is rejected. Installing this package alone does not acquire Catch2.

Provision installed Catch2 **3.8.1** or set `GREVIR_CATCH2_SOURCE_DIR` to an existing
3.8.1 source checkout for offline setup. Only explicit
`GREVIR_FETCH_TEST_DEPENDENCIES=ON` permits downloading the pinned,
SHA-256-verified archive during configuration. Test execution never downloads
anything. Shared interrupt models, old numeric-mode AVR mock globals and the
legacy runner remain unextracted.
