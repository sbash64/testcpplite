#include <sbash64/testcpplite/testcpplite.hpp>
#include <sstream>
#include <iostream>
#include <exception>
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

static void expects9223372036854775807Actual9223372036854775808(
    TestResult &result) {
    assertEqual(result, 9223372036854775807ULL, 9223372036854775808ULL);
}

static void expectsNegative2147483648ActualNegative2147483649(
    TestResult &result) {
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

static void assertEqual(
    TestResult &result, std::string_view s, const std::stringstream &stream) {
    assertEqual(result, s, stream.str());
}

static void assertEqual(
    TestResult &result, std::string_view s, const std::vector<Test> &tests) {
    assertEqual(result, s, testStream(tests));
}

static void assertEqual(TestResult &result, std::string_view s, const Test &t) {
    assertEqual(result, s, testStream({t}));
}

static auto putNewLine(std::ostream &stream) -> std::ostream & {
    return stream << '\n';
}

static auto putExpectationMessage(std::ostream &stream,
    std::string_view expected, std::string_view actual) -> std::ostream & {
    return putNewLine(
        putNewLine(putNewLine(putNewLine(stream << "expected:") << expected)
            << "actual:")
        << actual);
};

static auto putFailMessage(std::ostream &stream, std::string_view name)
    -> std::ostream & {
    return putNewLine(stream << "\x1b[31mfailed\x1b[0m " << name);
}

static auto putExpectsAActualBFailMessage(
    std::ostream &stream, std::string_view name) -> std::ostream & {
    return putExpectationMessage(
        putFailMessage(stream, name), "\"a\"", "\"b\"");
}

static auto putPassMessage(std::ostream &stream) -> std::ostream & {
    return putNewLine(stream << "\x1b[32mpassed\x1b[0m - 1 test");
}

static void passedOnlyTestShowsPassedMessage(TestResult &result) {
    std::stringstream stream;
    putPassMessage(stream);
    assertEqual(result, stream.str(), {passes, "myTest"});
}

static void passedReturnsZero(TestResult &result) {
    assertEqual(result, 0, test({{passes, "myTest"}}));
}

static void failedReturnsOne(TestResult &result) {
    assertEqual(result, 1, test({{fails, "myTest"}}));
}

static void failedOnlyTestShowsFailedMessage(TestResult &result) {
    std::stringstream stream;
    putExpectsAActualBFailMessage(stream, "myTest");
    assertEqual(result, stream.str(), {expectsAActualB, "myTest"});
}

static void failedOneOfTwoTestsShowsFailedMessage(TestResult &result) {
    std::stringstream stream;
    putExpectsAActualBFailMessage(stream, "myFailingTest");
    assertEqual(result, stream.str(),
        {{passes, "myTest"}, {expectsAActualB, "myFailingTest"}});
}

static void failsBothTestsShowsFailedMessage(TestResult &result) {
    std::stringstream stream;
    putExpectsAActualBFailMessage(
        putExpectsAActualBFailMessage(stream, "myFirstTest"), "mySecondTest");
    assertEqual(result, stream.str(),
        {{expectsAActualB, "myFirstTest"}, {expectsAActualB, "mySecondTest"}});
}

static void passesLastTestButFailsFirstShowsFailedMessage(TestResult &result) {
    std::stringstream stream;
    putExpectsAActualBFailMessage(stream, "myFailingTest");
    assertEqual(result, stream.str(),
        {{expectsAActualB, "myFailingTest"}, {passes, "myTest"}});
}

static void passedIntegerComparisonShowsPassedMessage(TestResult &result) {
    std::stringstream stream;
    putPassMessage(stream);
    assertEqual(result, stream.str(), {passesIntegerComparison, "myTest"});
}

static void passedIntegerPointerComparisonShowsPassedMessage(
    TestResult &result) {
    std::stringstream stream;
    putPassMessage(stream);
    assertEqual(
        result, stream.str(), {passesIntegerPointerComparison, "myTest"});
}

static void failedIntegerComparisonShowsFailedMessage(TestResult &result) {
    std::stringstream stream;
    putExpectationMessage(putFailMessage(stream, "myTest"), "1", "0");
    assertEqual(result, stream.str(), {expectsOneActualZero, "myTest"});
}

static void failedIntegerPointerComparisonShowsFailedMessage(
    TestResult &result) {
    const int a{0};
    const int b{0};
    const auto *const pa{&a};
    const auto *const pb{&b};
    std::stringstream expected;
    expected << pa;
    std::stringstream actual;
    actual << pb;
    std::stringstream stream;
    putExpectationMessage(
        putFailMessage(stream, "myTest"), expected.str(), actual.str());
    assertEqual(result, stream.str(),
        {[=](testcpplite::TestResult &subresult) {
             assertEqual(subresult, pa, pb);
         },
            "myTest"});
}

static void passedBooleanAssertionShowsPassedMessage(TestResult &result) {
    std::stringstream stream;
    putPassMessage(stream);
    assertEqual(result, stream.str(), {passesBooleanAssertion, "myTest"});
}

static void failedBooleanAssertionShowsFailedMessage(TestResult &result) {
    std::stringstream stream;
    putExpectationMessage(putFailMessage(stream, "myTest"), "true", "false");
    assertEqual(result, stream.str(), {expectsTrueActualFalse, "myTest"});
}

static void passedNegativeBooleanAssertionShowsPassedMessage(
    TestResult &result) {
    std::stringstream stream;
    putPassMessage(stream);
    assertEqual(
        result, stream.str(), {passesNegativeBooleanAssertion, "myTest"});
}

static void failedNegativeBooleanAssertionShowsFailedMessage(
    TestResult &result) {
    std::stringstream stream;
    putExpectationMessage(putFailMessage(stream, "myTest"), "false", "true");
    assertEqual(result, stream.str(), {expectsFalseActualTrue, "myTest"});
}

static void failedUnsignedIntegerComparisonShowsFailedMessage(
    TestResult &result) {
    std::stringstream stream;
    putExpectationMessage(
        putFailMessage(stream, "myTest"), "2147483647", "2147483648");
    assertEqual(
        result, stream.str(), {expects2147483647Actual2147483648, "myTest"});
}

static void failedLongComparisonShowsFailedMessage(TestResult &result) {
    std::stringstream stream;
    putExpectationMessage(putFailMessage(stream, "myTest"), "1", "2");
    assertEqual(result, stream.str(),
        {[](TestResult &result_) { assertEqual(result_, 1L, 2L); }, "myTest"});
}

static void failedReallyLargeUnsignedIntegerComparisonShowsFailedMessage(
    TestResult &result) {
    std::stringstream stream;
    putExpectationMessage(putFailMessage(stream, "myTest"),
        "9223372036854775807", "9223372036854775808");
    assertEqual(result, stream.str(),
        {expects9223372036854775807Actual9223372036854775808, "myTest"});
}

static void failedReallyLargeSignedIntegerComparisonShowsFailedMessage(
    TestResult &result) {
    std::stringstream stream;
    putExpectationMessage(
        putFailMessage(stream, "myTest"), "-2147483648", "-2147483649");
    assertEqual(result, stream.str(),
        {expectsNegative2147483648ActualNegative2147483649, "myTest"});
}

static void catchesStandardExceptions(TestResult &result) {
    std::stringstream stream;
    putNewLine(putFailMessage(stream, "myTest") << "error");
    assertEqual(result, stream.str(),
        {[](TestResult &) { throw StandardException{"error"}; }, "myTest"});
}

static void printsPassedTestCount(TestResult &result) {
    assertEqual(result, "\x1b[32mpassed\x1b[0m - 3 tests\n",
        {{passesBooleanAssertion, "a"}, {passesBooleanAssertion, "b"},
            {passesBooleanAssertion, "c"}});
}

static int main() {
    return testcpplite::test(
        {{passedOnlyTestShowsPassedMessage, "passedOnlyTestShowsPassedMessage"},
            {passedReturnsZero, "passedReturnsZero"},
            {failedReturnsOne, "failedReturnsOne"},
            {failedOnlyTestShowsFailedMessage,
                "failedOnlyTestShowsFailedMessage"},
            {failedOneOfTwoTestsShowsFailedMessage,
                "failedOneOfTwoTestsShowsFailedMessage"},
            {failsBothTestsShowsFailedMessage,
                "failsBothTestsShowsFailedMessage"},
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
            {printsPassedTestCount, "printsPassedTestCount"}},
        std::cout);
}
}

int main() { return sbash64::testcpplite::main(); }
