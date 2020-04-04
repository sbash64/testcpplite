add_executable(tests main.cpp)
target_link_libraries(tests testcpplite)
add_test(NAME tests COMMAND tests)
