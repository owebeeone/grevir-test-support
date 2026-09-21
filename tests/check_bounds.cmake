file(MAKE_DIRECTORY "${LOG_DIR}")
foreach(case RANGE 0 3)
  execute_process(COMMAND "${CXX}" -std=c++23 "-I${INCLUDE_DIR}"
    "-DCASE_ID=${case}" -fsyntax-only "${SOURCE}"
    RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE errors)
  file(WRITE "${LOG_DIR}/case-${case}.log" "${output}${errors}")
  if(case EQUAL 0)
    if(NOT result STREQUAL "0")
      message(FATAL_ERROR "Valid debug register failed: ${errors}")
    endif()
  elseif(NOT result MATCHES "^[1-9][0-9]*$" OR
      NOT errors MATCHES "static assertion failed[^\n]*GREVIR_DEBUG_REGISTER_OUT_OF_BOUNDS")
    message(FATAL_ERROR "Expected bounds diagnostic for case ${case}: ${errors}")
  endif()
endforeach()
message(STATUS "Debug register bounds: 1 valid and 3 expected rejections passed")
