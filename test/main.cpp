#include <testcpplite/testcpplite.hpp>
#include <sstream>
#include <iostream>

namespace testcpplite {
namespace {
void passes(TestResult &result) { assertEqual(result, "a", "a"); }

void expectsAActualB(TestResult &result) { assertEqual(result, "a", "b"); }

void fails(TestResult &result) { expectsAActualB(result); }

void passesIntegerComparison(TestResult &result) { assertEqual(result, 1, 1); }

void expectsOneActualZero(TestResult &result) { assertEqual(result, 1, 0); }

void passesBooleanAssertion(TestResult &result) { assertTrue(result, true); }

void expectsTrueActualFalse(TestResult &result) { assertTrue(result, false); }

void passesNegativeBooleanAssertion(TestResult &result) {
    assertFalse(result, false);
}

void expectsFalseActualTrue(TestResult &result) { assertFalse(result, true); }

auto testStream(const std::vector<Test> &tests) -> std::stringstream {
    std::stringstream stream;
    test(tests, stream);
    return stream;
}

auto test(const std::vector<Test> &tests) -> bool {
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

auto passMessage() -> std::string { return withNewLine("pass"); }

void passedOnlyTestShowsPassedMessage(TestResult &result) {
    assertEqual(result, passMessage(), {passes, "myTest"});
}

void passedReturnsTrue(TestResult &result) {
    assertTrue(result, test({{passes, "myTest"}}));
}

void failedReturnsFalse(TestResult &result) {
    assertFalse(result, test({{fails, "myTest"}}));
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

void failedIntegerComparisonShowsFailedMessage(TestResult &result) {
    assertEqual(result, failMessage("myTest") + expectationMessage("1", "0"),
        {expectsOneActualZero, "myTest"});
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

void main() {
    testcpplite::test(
        {{passedOnlyTestShowsPassedMessage, "passedOnlyTestShowsPassedMessage"},
            {passedReturnsTrue, "passedReturnsTrue"},
            {failedReturnsFalse, "failedReturnsFalse"},
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
            {failedIntegerComparisonShowsFailedMessage,
                "failedIntegerComparisonShowsFailedMessage"},
            {passedBooleanAssertionShowsPassedMessage,
                "passedBooleanAssertionShowsPassedMessage"},
            {failedBooleanAssertionShowsFailedMessage,
                "failedBooleanAssertionShowsFailedMessage"},
            {passedNegativeBooleanAssertionShowsPassedMessage,
                "passedNegativeBooleanAssertionShowsPassedMessage"},
            {failedNegativeBooleanAssertionShowsFailedMessage,
                "failedNegativeBooleanAssertionShowsFailedMessage"}},
        std::cout);
}
}
}

int main() { testcpplite::main(); }
