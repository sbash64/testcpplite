#ifndef SBASH64_TESTCPPLITE_LIB_INCLUDE_SBASH64_TESTCPPLITE_TESTCPPLITE_HPP_
#define SBASH64_TESTCPPLITE_LIB_INCLUDE_SBASH64_TESTCPPLITE_TESTCPPLITE_HPP_

#include <functional>
#include <ostream>
#include <string_view>
#include <vector>

namespace sbash64::testcpplite {
struct TestResult;

struct Test {
  std::function<void(TestResult &)> f;
  std::string name;
};

auto test(const std::vector<Test> &tests, std::ostream &) -> int;
void assertEqual(TestResult &, std::string_view expected,
                 std::string_view actual);
void assertEqual(TestResult &, int expected, int actual);
void assertEqual(TestResult &, long expected, long actual);
void assertEqual(TestResult &, long long expected, long long actual);
void assertEqual(TestResult &, unsigned long expected, unsigned long actual);
void assertEqual(TestResult &, unsigned long long expected,
                 unsigned long long actual);
void assertEqual(TestResult &, const void *expected, const void *actual);
void assertTrue(TestResult &, bool);
void assertFalse(TestResult &, bool);
} // namespace sbash64::testcpplite

#endif
