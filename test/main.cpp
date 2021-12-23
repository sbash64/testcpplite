#include <sbash64/testcpplite/testcpplite.hpp>

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

using std::literals::string_literals::operator""s;

namespace sbash64::testcpplite {
static void passes(TestResult &result) { assertEqual(result, "a"s, "a"s); }

static void expectsAActualB(TestResult &result) {
  assertEqual(result, "a"s, "b"s);
}

static void fails(TestResult &result) { expectsAActualB(result); }

static void passesIntegerComparison(TestResult &result) {
  assertEqual(result, 1, 1);
}

static void passesIntegerPointerComparison(TestResult &result) {
  const int b{0};
  assertEqual(result, &b, &b);
}

static void expects2147483647Actual2147483648(TestResult &result) {
  assertEqual(result, 2147483647UL, 2147483648UL);
}

static void
expects9223372036854775807Actual9223372036854775808(TestResult &result) {
  assertEqual(result, 9223372036854775807ULL, 9223372036854775808ULL);
}

static void
expectsNegative2147483648ActualNegative2147483649(TestResult &result) {
  assertEqual(result, -2147483648LL, -2147483649LL);
}

static void expectsOneActualZero(TestResult &result) {
  assertEqual(result, 1, 0);
}

static void passesBooleanAssertion(TestResult &result) {
  assertTrue(result, true);
}

static void expectsTrueActualFalse(TestResult &result) {
  assertTrue(result, false);
}

static void passesNegativeBooleanAssertion(TestResult &result) {
  assertFalse(result, false);
}

static void expectsFalseActualTrue(TestResult &result) {
  assertFalse(result, true);
}

namespace {
class StandardException : public std::exception {
public:
  explicit StandardException(std::string s) : s{std::move(s)} {}

  [[nodiscard]] auto what() const noexcept -> const char * override {
    return s.c_str();
  }

private:
  std::string s;
};
} // namespace

static void assertEqual(TestResult &result, const std::stringstream &expected,
                        const std::stringstream &actual) {
  assertEqual(result, expected.str(), actual.str());
}

static auto testStream(const std::vector<Test> &tests) -> std::stringstream {
  std::stringstream stream;
  test(tests, stream);
  return stream;
}

static auto test(const std::vector<Test> &tests) -> int {
  std::stringstream stream;
  return test(tests, stream);
}

static void assertEqual(TestResult &result, const std::stringstream &stream,
                        const std::vector<Test> &tests) {
  assertEqual(result, stream, testStream(tests));
}

static void assertEqual(TestResult &result, const std::stringstream &stream,
                        const Test &t) {
  assertEqual(result, stream, std::vector<Test>{t});
}

static auto putNewLine(std::ostream &stream) -> std::ostream & {
  return stream << '\n';
}

static auto putExpectationMessage(std::ostream &stream,
                                  std::string_view expected,
                                  std::string_view actual) -> std::ostream & {
  return putNewLine(
      putNewLine(putNewLine(putNewLine(stream << "expected:") << expected)
                 << "actual:")
      << actual);
};

static auto putFailMessage(std::ostream &stream, std::string_view name)
    -> std::ostream & {
  return putNewLine(stream << "\x1b[31mfailed\x1b[0m " << name);
}

static auto putExpectsAActualBFailMessage(std::ostream &stream,
                                          std::string_view name)
    -> std::ostream & {
  return putExpectationMessage(putFailMessage(stream, name), "\"a\"", "\"b\"");
}

static auto putPassMessage(std::ostream &stream) -> std::ostream & {
  return putNewLine(stream << "\x1b[32mpassed\x1b[0m - 1 test");
}

static void
expectTestStreamMatches(TestResult &result,
                        const std::function<void(std::stringstream &)> &f,
                        const Test &t) {
  std::stringstream stream;
  f(stream);
  assertEqual(result, stream, t);
}

static void
expectTestStreamMatches(TestResult &result,
                        const std::function<void(std::stringstream &)> &f,
                        const std::vector<Test> &t) {
  std::stringstream stream;
  f(stream);
  assertEqual(result, stream, t);
}

static void passedOnlyTestShowsPassedMessage(TestResult &result) {
  expectTestStreamMatches(
      result, [](std::stringstream &stream) { putPassMessage(stream); },
      {passes, "a test"});
}

static void passedReturnsZero(TestResult &result) {
  assertEqual(result, 0, test({{passes, "a test"}}));
}

static void failedReturnsOne(TestResult &result) {
  assertEqual(result, 1, test({{fails, "a test"}}));
}

static void failedOnlyTestShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(result,
                          [](std::stringstream &stream) {
                            putExpectsAActualBFailMessage(stream, "a test");
                          },
                          {expectsAActualB, "a test"});
}

static void failedOneOfTwoTestsShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(
      result,
      [](std::stringstream &stream) {
        putExpectsAActualBFailMessage(stream, "failing test");
      },
      {{passes, "a test"}, {expectsAActualB, "failing test"}});
}

static void failsBothTestsShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(
      result,
      [](std::stringstream &stream) {
        putExpectsAActualBFailMessage(
            putExpectsAActualBFailMessage(stream, "first test"), "second test");
      },
      {{expectsAActualB, "first test"}, {expectsAActualB, "second test"}});
}

static void passesLastTestButFailsFirstShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(
      result,
      [](std::stringstream &stream) {
        putExpectsAActualBFailMessage(stream, "failing test");
      },
      {{expectsAActualB, "failing test"}, {passes, "a test"}});
}

static void passedIntegerComparisonShowsPassedMessage(TestResult &result) {
  expectTestStreamMatches(
      result, [](std::stringstream &stream) { putPassMessage(stream); },
      {passesIntegerComparison, "a test"});
}

static void
passedIntegerPointerComparisonShowsPassedMessage(TestResult &result) {
  expectTestStreamMatches(
      result, [](std::stringstream &stream) { putPassMessage(stream); },
      {passesIntegerPointerComparison, "a test"});
}

static void failedIntegerComparisonShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(result,
                          [](std::stringstream &stream) {
                            putExpectationMessage(
                                putFailMessage(stream, "a test"), "1", "0");
                          },
                          {expectsOneActualZero, "a test"});
}

static void twoAssertionFailures(TestResult &result) {
  expectTestStreamMatches(
      result,
      [](std::stringstream &stream) {
        putExpectationMessage(putNewLine(putExpectationMessage(
                                  putFailMessage(stream, "a test"), "0", "1")),
                              "4", "5");
      },
      {[=](testcpplite::TestResult &subresult) {
         assertEqual(subresult, 0, 1);
         assertEqual(subresult, 4, 5);
       },
       "a test"});
}

static void
failedIntegerPointerComparisonShowsFailedMessage(TestResult &result) {
  const int a{0};
  const int b{0};
  const auto *const pa{&a};
  const auto *const pb{&b};
  expectTestStreamMatches(result,
                          [=](std::stringstream &stream) {
                            std::stringstream expected;
                            expected << pa;
                            std::stringstream actual;
                            actual << pb;
                            putExpectationMessage(
                                putFailMessage(stream, "a test"),
                                expected.str(), actual.str());
                          },
                          {[=](testcpplite::TestResult &subresult) {
                             assertEqual(subresult, pa, pb);
                           },
                           "a test"});
}

static void passedBooleanAssertionShowsPassedMessage(TestResult &result) {
  expectTestStreamMatches(
      result, [](std::stringstream &stream) { putPassMessage(stream); },
      {passesBooleanAssertion, "a test"});
}

static void failedBooleanAssertionShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(result,
                          [](std::stringstream &stream) {
                            putExpectationMessage(
                                putFailMessage(stream, "a test"), "true",
                                "false");
                          },
                          {expectsTrueActualFalse, "a test"});
}

static void
passedNegativeBooleanAssertionShowsPassedMessage(TestResult &result) {
  expectTestStreamMatches(
      result, [](std::stringstream &stream) { putPassMessage(stream); },
      {passesNegativeBooleanAssertion, "a test"});
}

static void
failedNegativeBooleanAssertionShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(result,
                          [](std::stringstream &stream) {
                            putExpectationMessage(
                                putFailMessage(stream, "a test"), "false",
                                "true");
                          },
                          {expectsFalseActualTrue, "a test"});
}

static void
failedUnsignedIntegerComparisonShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(result,
                          [=](std::stringstream &stream) {
                            putExpectationMessage(
                                putFailMessage(stream, "a test"), "2147483647",
                                "2147483648");
                          },
                          {expects2147483647Actual2147483648, "a test"});
}

static void failedLongComparisonShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(
      result,
      [=](std::stringstream &stream) {
        putExpectationMessage(putFailMessage(stream, "a test"), "1", "2");
      },
      {[](TestResult &result_) { assertEqual(result_, 1L, 2L); }, "a test"});
}

static void failedReallyLargeUnsignedIntegerComparisonShowsFailedMessage(
    TestResult &result) {
  expectTestStreamMatches(
      result,
      [=](std::stringstream &stream) {
        putExpectationMessage(putFailMessage(stream, "a test"),
                              "9223372036854775807", "9223372036854775808");
      },
      {expects9223372036854775807Actual9223372036854775808, "a test"});
}

static void
failedReallyLargeSignedIntegerComparisonShowsFailedMessage(TestResult &result) {
  expectTestStreamMatches(
      result,
      [=](std::stringstream &stream) {
        putExpectationMessage(putFailMessage(stream, "a test"), "-2147483648",
                              "-2147483649");
      },
      {expectsNegative2147483648ActualNegative2147483649, "a test"});
}

static void catchesStandardExceptions(TestResult &result) {
  expectTestStreamMatches(
      result,
      [=](std::stringstream &stream) {
        putNewLine(putFailMessage(stream, "a test") << "error");
      },
      {[](TestResult &) { throw StandardException{"error"}; }, "a test"});
}

static void printsPassedTestCount(TestResult &result) {
  expectTestStreamMatches(result,
                          [=](std::stringstream &stream) {
                            putNewLine(stream
                                       << "\x1b[32mpassed\x1b[0m - 3 tests");
                          },
                          {{passesBooleanAssertion, "a"},
                           {passesBooleanAssertion, "b"},
                           {passesBooleanAssertion, "c"}});
}

static int main() {
  return testcpplite::test(
      {{passedOnlyTestShowsPassedMessage, "passedOnlyTestShowsPassedMessage"},
       {passedReturnsZero, "passedReturnsZero"},
       {failedReturnsOne, "failedReturnsOne"},
       {failedOnlyTestShowsFailedMessage, "failedOnlyTestShowsFailedMessage"},
       {failedOneOfTwoTestsShowsFailedMessage,
        "failedOneOfTwoTestsShowsFailedMessage"},
       {failsBothTestsShowsFailedMessage, "failsBothTestsShowsFailedMessage"},
       {passesLastTestButFailsFirstShowsFailedMessage,
        "passesLastTestButFailsFirstShowsFailedMessage"},
       {passedIntegerComparisonShowsPassedMessage,
        "passedIntegerComparisonShowsPassedMessage"},
       {passedIntegerPointerComparisonShowsPassedMessage,
        "passedIntegerPointerComparisonShowsPassedMessage"},
       {failedIntegerComparisonShowsFailedMessage,
        "failedIntegerComparisonShowsFailedMessage"},
       {failedIntegerPointerComparisonShowsFailedMessage,
        "failedIntegerPointerComparisonShowsFailedMessage"},
       {passedBooleanAssertionShowsPassedMessage,
        "passedBooleanAssertionShowsPassedMessage"},
       {failedBooleanAssertionShowsFailedMessage,
        "failedBooleanAssertionShowsFailedMessage"},
       {passedNegativeBooleanAssertionShowsPassedMessage,
        "passedNegativeBooleanAssertionShowsPassedMessage"},
       {failedNegativeBooleanAssertionShowsFailedMessage,
        "failedNegativeBooleanAssertionShowsFailedMessage"},
       {failedUnsignedIntegerComparisonShowsFailedMessage,
        "failedUnsignedIntegerComparisonShowsFailedMessage"},
       {failedLongComparisonShowsFailedMessage,
        "failedLongComparisonShowsFailedMessage"},
       {failedReallyLargeUnsignedIntegerComparisonShowsFailedMessage,
        "failedReallyLargeUnsignedIntegerComparisonShowsFailedMessage"},
       {failedReallyLargeSignedIntegerComparisonShowsFailedMessage,
        "failedReallyLargeSignedIntegerComparisonShowsFailedMessage"},
       {catchesStandardExceptions, "catchesStandardExceptions"},
       {printsPassedTestCount, "printsPassedTestCount"},
       {twoAssertionFailures, "twoAssertionFailures"}},
      std::cout);
}
} // namespace sbash64::testcpplite

int main() { return sbash64::testcpplite::main(); }
