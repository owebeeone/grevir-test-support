cmake_minimum_required(VERSION 3.20)
project(grevir_test_support VERSION 0.1.0 LANGUAGES CXX)

# Development-only helpers. Installing this package does not acquire Catch2.
include(GNUInstallDirs)
add_library(grevir_test_support INTERFACE)
add_library(grevir::test_support ALIAS grevir_test_support)
set_target_properties(grevir_test_support PROPERTIES EXPORT_NAME test_support)
target_compile_features(grevir_test_support INTERFACE cxx_std_23)
target_include_directories(grevir_test_support INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
install(TARGETS grevir_test_support EXPORT GrevirTestSupportTargets)
install(EXPORT GrevirTestSupportTargets NAMESPACE grevir::
  DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/grevir-test-support)
install(DIRECTORY include/ DESTINATION include)
install(FILES cmake/GrevirTesting.cmake cmake/grevir-test-support-config.cmake
  DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/grevir-test-support)
install(FILES LICENSE.txt DESTINATION ${CMAKE_INSTALL_DATADIR}/grevir-test-support)

option(GREVIR_BUILD_HOST_TESTS "Run deterministic native behavior tests" OFF)
if(GREVIR_BUILD_HOST_TESTS)
  enable_testing()
  include(cmake/GrevirTesting.cmake)
  add_subdirectory(tests)
endif()
