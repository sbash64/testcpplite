cmake_minimum_required(VERSION 3.10.0)
project(
  TestCppLite
  VERSION 2.1.2
  LANGUAGES CXX)
add_subdirectory(lib)

option(SBASH64_TESTCPPLITE_ENABLE_TESTS "Enable tests" OFF)
if(${SBASH64_TESTCPPLITE_ENABLE_TESTS})
  enable_testing()
  add_subdirectory(test)
endif()
