include("${CMAKE_CURRENT_LIST_DIR}/GrevirTestSupportTargets.cmake")
# Fixture-only consumers need no test runner. Existing host suites set this option.
if(GREVIR_BUILD_HOST_TESTS)
  include("${CMAKE_CURRENT_LIST_DIR}/GrevirTesting.cmake")
endif()
