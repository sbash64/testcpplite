#ifndef TESTCPPLITE_SRC_INCLUDE_TESTCPPLITE_TESTCPPLITE_HPP_
#define TESTCPPLITE_SRC_INCLUDE_TESTCPPLITE_TESTCPPLITE_HPP_

#include <string>
#include <vector>
#include <functional>
#include <ostream>

namespace testcpplite {
struct TestResult;

struct Test {
    std::function<void(TestResult &)> f;
    std::string name;
};

auto test(const std::vector<Test> &tests, std::ostream &) -> bool;
void assertEqual(
    TestResult &, const std::string &expected, const std::string &actual);
void assertEqual(TestResult &, int expected, int actual);
void assertTrue(TestResult &, bool);
void assertFalse(TestResult &, bool);
}

#endif
