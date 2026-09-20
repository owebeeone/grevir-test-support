include_guard(GLOBAL)

if(CMAKE_CROSSCOMPILING)
  message(FATAL_ERROR "GREVIR_BUILD_HOST_TESTS requires a native compiler")
endif()

option(GREVIR_FETCH_TEST_DEPENDENCIES "Allow setup to download pinned host test dependencies" OFF)
set(GREVIR_CATCH2_SOURCE_DIR "" CACHE PATH "Existing Catch2 3.8.1 source for offline setup")
if(GREVIR_CATCH2_SOURCE_DIR)
  add_subdirectory("${GREVIR_CATCH2_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/catch2" EXCLUDE_FROM_ALL)
  include("${GREVIR_CATCH2_SOURCE_DIR}/extras/Catch.cmake")
else()
  find_package(Catch2 3.8.1 EXACT CONFIG QUIET)
  if(Catch2_FOUND)
    set_property(TARGET Catch2::Catch2 Catch2::Catch2WithMain PROPERTY IMPORTED_GLOBAL TRUE)
    include("${Catch2_DIR}/Catch.cmake")
  else()
    if(NOT GREVIR_FETCH_TEST_DEPENDENCIES)
      message(FATAL_ERROR
        "Host tests need Catch2 3.8.1. Supply an installed package, set GREVIR_CATCH2_SOURCE_DIR, or explicitly enable GREVIR_FETCH_TEST_DEPENDENCIES for setup.")
    endif()
    include(FetchContent)
    if(POLICY CMP0135)
      cmake_policy(SET CMP0135 NEW)
    endif()
    FetchContent_Declare(Catch2
      URL https://codeload.github.com/catchorg/Catch2/tar.gz/refs/tags/v3.8.1
      URL_HASH SHA256=18b3f70ac80fccc340d8c6ff0f339b2ae64944782f8d2fca2bd705cf47cadb79
      TLS_VERIFY TRUE)
    FetchContent_MakeAvailable(Catch2)
    include("${catch2_SOURCE_DIR}/extras/Catch.cmake")
  endif()
endif()

