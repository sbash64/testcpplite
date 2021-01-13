#ifndef SBASH64_TESTCPPLITE_LIB_INCLUDE_SBASH64_TESTCPPLITE_TESTCPPLITE_HPP_
#define SBASH64_TESTCPPLITE_LIB_INCLUDE_SBASH64_TESTCPPLITE_TESTCPPLITE_HPP_

#include <string>
#include <vector>
#include <functional>
#include <ostream>

namespace sbash64 {
namespace testcpplite {
struct TestResult;

struct Test {
    std::function<void(TestResult &)> f;
    std::string name;
};

auto test(const std::vector<Test> &tests, std::ostream &) -> int;
void assertEqual(
    TestResult &, const std::string &expected, const std::string &actual);
void assertEqual(TestResult &, int expected, int actual);
void assertEqual(TestResult &, long expected, long actual);
void assertEqual(TestResult &, long long expected, long long actual);
void assertEqual(TestResult &, const void *expected, const void *actual);
void assertEqual(TestResult &, unsigned long expected, unsigned long actual);
void assertEqual(
    TestResult &, unsigned long long expected, unsigned long long actual);
void assertTrue(TestResult &, bool);
void assertFalse(TestResult &, bool);
}
}

#endif
