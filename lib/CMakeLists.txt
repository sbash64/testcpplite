add_library(sbash64-testcpplite testcpplite.cpp)
target_include_directories(
  sbash64-testcpplite
  PRIVATE include/sbash64/testcpplite
  PUBLIC include)
target_compile_features(sbash64-testcpplite PUBLIC cxx_std_17)
target_compile_options(sbash64-testcpplite
                       PRIVATE ${SBASH64_TESTCPPLITE_WARNINGS})
set_target_properties(sbash64-testcpplite PROPERTIES CXX_EXTENSIONS OFF)
