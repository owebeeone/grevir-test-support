add_executable(grevir_register_memory_test register_memory_test.cpp)
target_link_libraries(grevir_register_memory_test PRIVATE grevir::test_support Catch2::Catch2WithMain)
set_target_properties(grevir_register_memory_test PROPERTIES CXX_EXTENSIONS OFF)
catch_discover_tests(grevir_register_memory_test TEST_PREFIX "test-support."
  PROPERTIES LABELS "test-support" TIMEOUT 10)
if(CMAKE_CXX_COMPILER_ID MATCHES "^(AppleClang|Clang|GNU|MSVC)$")
  add_custom_target(grevir_debug_register_bounds ALL
    COMMAND "${CMAKE_COMMAND}" "-DCXX=${CMAKE_CXX_COMPILER}"
      "-DCOMPILER_ID=${CMAKE_CXX_COMPILER_ID}"
      "-DINCLUDE_DIR=${PROJECT_SOURCE_DIR}/include"
      "-DSOURCE=${CMAKE_CURRENT_SOURCE_DIR}/bounds_probe.cpp"
      "-DLOG_DIR=${CMAKE_CURRENT_BINARY_DIR}/bounds-results"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/check_bounds.cmake"
    VERBATIM)
endif()
