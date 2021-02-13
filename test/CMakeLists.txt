add_executable(sbash64-testcpplite-tests main.cpp)
target_link_libraries(sbash64-testcpplite-tests sbash64-testcpplite)
target_compile_options(sbash64-testcpplite-tests
                       PRIVATE ${SBASH64_TESTCPPLITE_WARNINGS})
set_target_properties(sbash64-testcpplite-tests PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(sbash64-testcpplite-tests PRIVATE cxx_std_17)
add_test(NAME sbash64-testcpplite-tests COMMAND sbash64-testcpplite-tests)
