#include <sbash64/testcpplite/testcpplite.hpp>
#include <sstream>
#include <iostream>
#include <exception>
#include <string>

namespace sbash64::testcpplite {
namespace {
void passes(TestResult &result) {
    assertEqual(result, std::string{"a"}, std::string{"a"});
}

void expectsAActualB(TestResult &result) {
    assertEqual(result, std::string{"a"}, std::string{"b"});
}

void fails(TestResult &result) { expectsAActualB(result); }

void passesIntegerComparison(TestResult &result) { assertEqual(result, 1, 1); }

void passesIntegerPointerComparison(TestResult &result) {
    const int b{0};
    assertEqual(result, &b, &b);
}

void expects2147483647Actual2147483648(TestResult &result) {
    assertEqual(result, 2147483647UL, 2147483648UL);
}

void expects9223372036854775807Actual9223372036854775808(TestResult &result) {
    assertEqual(result, 9223372036854775807ULL, 9223372036854775808ULL);
}

void expectsNegative2147483648ActualNegative2147483649(TestResult &result) {
    assertEqual(result, -2147483648LL, -2147483649LL);
}

void expectsOneActualZero(TestResult &result) { assertEqual(result, 1, 0); }

void passesBooleanAssertion(TestResult &result) { assertTrue(result, true); }

void expectsTrueActualFalse(TestResult &result) { assertTrue(result, false); }

void passesNegativeBooleanAssertion(TestResult &result) {
    assertFalse(result, false);
}

void expectsFalseActualTrue(TestResult &result) { assertFalse(result, true); }

class StandardException : public std::exception {
  public:
    explicit StandardException(std::string s) : s{std::move(s)} {}

    [[nodiscard]] auto what() const noexcept -> const char * override {
        return s.c_str();
    }

  private:
    std::string s;
};

auto testStream(const std::vector<Test> &tests) -> std::stringstream {
    std::stringstream stream;
    test(tests, stream);
    return stream;
}

auto test(const std::vector<Test> &tests) -> int {
    std::stringstream stream;
    return test(tests, stream);
}

void assertEqual(
    TestResult &result, const std::string &s, const std::stringstream &stream) {
    testcpplite::assertEqual(result, s, stream.str());
}

void assertEqual(
    TestResult &result, const std::string &s, const std::vector<Test> &tests) {
    assertEqual(result, s, testStream(tests));
}

void assertEqual(TestResult &result, const std::string &s, const Test &t) {
    assertEqual(result, s, testStream({t}));
}

auto withNewLine(const std::string &s) -> std::string { return s + '\n'; }

auto expectationMessage(const std::string &expected, const std::string &actual)
    -> std::string {
    return withNewLine("    expected " + expected + ", actual " + actual);
};

auto failMessage(const std::string &name) -> std::string {
    return withNewLine("fail " + name);
}

auto failsExpectsAActualBMessage(const std::string &name) -> std::string {
    return failMessage(name) + expectationMessage("\"a\"", "\"b\"");
}

auto passMessage() -> std::string { return withNewLine("pass - 1 test"); }

void passedOnlyTestShowsPassedMessage(TestResult &result) {
    assertEqual(result, passMessage(), {passes, "myTest"});
}

void passedReturnsZero(TestResult &result) {
    assertEqual(result, 0, test({{passes, "myTest"}}));
}

void failedReturnsOne(TestResult &result) {
    assertEqual(result, 1, test({{fails, "myTest"}}));
}

void failedOnlyTestShowsFailedMessage(TestResult &result) {
    assertEqual(result, failsExpectsAActualBMessage("myTest"),
        {expectsAActualB, "myTest"});
}

void failedOneOfTwoTestsShowsFailedMessage(TestResult &result) {
    assertEqual(result, failsExpectsAActualBMessage("myFailingTest"),
        {{passes, "myTest"}, {expectsAActualB, "myFailingTest"}});
}

void failsBothTestsShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        failsExpectsAActualBMessage("myFirstTest") +
            failsExpectsAActualBMessage("mySecondTest"),
        {{expectsAActualB, "myFirstTest"}, {expectsAActualB, "mySecondTest"}});
}

void passesLastTestButFailsFirstShowsFailedMessage(TestResult &result) {
    assertEqual(result, failsExpectsAActualBMessage("myFailingTest"),
        {{expectsAActualB, "myFailingTest"}, {passes, "myTest"}});
}

void passedIntegerComparisonShowsPassedMessage(TestResult &result) {
    assertEqual(result, passMessage(), {passesIntegerComparison, "myTest"});
}

void passedIntegerPointerComparisonShowsPassedMessage(TestResult &result) {
    assertEqual(
        result, passMessage(), {passesIntegerPointerComparison, "myTest"});
}

void failedIntegerComparisonShowsFailedMessage(TestResult &result) {
    assertEqual(result, failMessage("myTest") + expectationMessage("1", "0"),
        {expectsOneActualZero, "myTest"});
}

void failedIntegerPointerComparisonShowsFailedMessage(TestResult &result) {
    const int a{0};
    const int b{0};
    const auto *const pa{&a};
    const auto *const pb{&b};
    std::stringstream expected;
    expected << pa;
    std::stringstream actual;
    actual << pb;
    assertEqual(result,
        failMessage("myTest") +
            expectationMessage(expected.str(), actual.str()),
        {[=](testcpplite::TestResult &subresult) {
             assertEqual(subresult, pa, pb);
         },
            "myTest"});
}

void passedBooleanAssertionShowsPassedMessage(TestResult &result) {
    assertEqual(result, passMessage(), {passesBooleanAssertion, "myTest"});
}

void failedBooleanAssertionShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        failMessage("myTest") + expectationMessage("true", "false"),
        {expectsTrueActualFalse, "myTest"});
}

void passedNegativeBooleanAssertionShowsPassedMessage(TestResult &result) {
    assertEqual(
        result, passMessage(), {passesNegativeBooleanAssertion, "myTest"});
}

void failedNegativeBooleanAssertionShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        failMessage("myTest") + expectationMessage("false", "true"),
        {expectsFalseActualTrue, "myTest"});
}

void failedUnsignedIntegerComparisonShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        failMessage("myTest") + expectationMessage("2147483647", "2147483648"),
        {expects2147483647Actual2147483648, "myTest"});
}

void failedLongComparisonShowsFailedMessage(TestResult &result) {
    assertEqual(result, failMessage("myTest") + expectationMessage("1", "2"),
        {[](TestResult &result_) { assertEqual(result_, 1L, 2L); }, "myTest"});
}

void failedReallyLargeUnsignedIntegerComparisonShowsFailedMessage(
    TestResult &result) {
    assertEqual(result,
        failMessage("myTest") +
            expectationMessage("9223372036854775807", "9223372036854775808"),
        {expects9223372036854775807Actual9223372036854775808, "myTest"});
}

void failedReallyLargeSignedIntegerComparisonShowsFailedMessage(
    TestResult &result) {
    assertEqual(result,
        failMessage("myTest") +
            expectationMessage("-2147483648", "-2147483649"),
        {expectsNegative2147483648ActualNegative2147483649, "myTest"});
}

void catchesStandardExceptions(TestResult &result) {
    assertEqual(result, failMessage("myTest") + "    error\n",
        {[](TestResult &) { throw StandardException{"error"}; }, "myTest"});
}

void printsPassedTestCount(TestResult &result) {
    assertEqual(result, "pass - 3 tests\n",
        {{passesBooleanAssertion, "a"}, {passesBooleanAssertion, "b"},
            {passesBooleanAssertion, "c"}});
}

int main() {
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
}

int main() { return sbash64::testcpplite::main(); }
