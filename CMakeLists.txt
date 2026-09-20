cmake_minimum_required(VERSION 3.20)
project(grevir_test_support VERSION 0.1.0 LANGUAGES NONE)

# Development-only helpers. Installing this package does not acquire Catch2.
include(GNUInstallDirs)
install(FILES cmake/GrevirTesting.cmake cmake/grevir-test-support-config.cmake
  DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/grevir-test-support)
install(FILES LICENSE.txt DESTINATION ${CMAKE_INSTALL_DATADIR}/grevir-test-support)

option(GREVIR_BUILD_HOST_TESTS "Run deterministic native behavior tests" OFF)
if(GREVIR_BUILD_HOST_TESTS)
  include(cmake/GrevirTesting.cmake)
endif()
