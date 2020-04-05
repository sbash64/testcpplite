add_library(testcpplite testcpplite.cpp)
target_include_directories(testcpplite PRIVATE include/testcpplite PUBLIC include)
target_compile_features(testcpplite PUBLIC cxx_std_11)
 