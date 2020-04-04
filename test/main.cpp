#include <testcpp/testcpp.hpp>
#include <sstream>
#include <iostream>

namespace testcpp {
namespace {
void passes(TestResult &result) { assertEqual(result, "a", "a"); }

void expectsAActualB(TestResult &result) { assertEqual(result, "a", "b"); }

void passesIntegerComparison(TestResult &result) { assertEqual(result, 1, 1); }

void failsIntegerComparison(TestResult &result) { assertEqual(result, 1, 0); }

void passesBooleanAssertion(TestResult &result) { assertTrue(result, true); }

void failsBooleanAssertion(TestResult &result) { assertTrue(result, false); }

void passesNegativeBooleanAssertion(TestResult &result) {
    assertFalse(result, false);
}

void failsNegativeBooleanAssertion(TestResult &result) {
    assertFalse(result, true);
}

auto test(const std::vector<Test> &tests) -> std::stringstream {
    std::stringstream stream;
    test(tests, stream);
    return stream;
}

void assertEqual(
    TestResult &result, const std::string &s, const std::stringstream &stream) {
    testcpp::assertEqual(result, s, stream.str());
}

void assertEqual(
    TestResult &result, const std::string &s, const std::vector<Test> &tests) {
    assertEqual(result, s, test(tests));
}

void assertEqual(TestResult &result, const std::string &s, const Test &t) {
    assertEqual(result, s, test({t}));
}

auto expectationMessage(const std::string &expected, const std::string &actual)
    -> std::string {
    return "    expected " + expected + ", actual " + actual + "\n";
};

auto expectsAActualBMessage() -> std::string {
    return expectationMessage("\"a\"", "\"b\"");
};

void passedOnlyTestShowsPassedMessage(TestResult &result) {
    assertEqual(result, "pass\n", {passes, "passes"});
}

void failedOnlyTestShowsFailedMessage(TestResult &result) {
    assertEqual(result, "fail fails\n" + expectsAActualBMessage(),
        {expectsAActualB, "fails"});
}

void failedOneOfTwoTestsShowsFailedMessage(TestResult &result) {
    assertEqual(result, "fail fails\n" + expectsAActualBMessage(),
        {{passes, "passes"}, {expectsAActualB, "fails"}});
}

void failsBothTestsShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        "fail fail1\n" + expectsAActualBMessage() +
            "fail fail2\n    "
            "expected \"a\", actual \"b\"\n",
        {{expectsAActualB, "fail1"}, {expectsAActualB, "fail2"}});
}

void passesLastTestButFailsFirstShowsFailedMessage(TestResult &result) {
    assertEqual(result, "fail fails\n" + expectsAActualBMessage(),
        {{expectsAActualB, "fails"}, {passes, "passes"}});
}

void passedIntegerComparisonShowsPassedMessage(TestResult &result) {
    assertEqual(
        result, "pass\n", {passesIntegerComparison, "integerComparisonPass"});
}

void failedIntegerComparisonShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        "fail failsIntegerComparison\n" + expectationMessage("1", "0"),
        {failsIntegerComparison, "failsIntegerComparison"});
}

void passedBooleanAssertionShowsPassedMessage(TestResult &result) {
    assertEqual(
        result, "pass\n", {passesBooleanAssertion, "passesBooleanAssertion"});
}

void failedBooleanAssertionShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        "fail failsBooleanAssertion\n" + expectationMessage("true", "false"),
        {failsBooleanAssertion, "failsBooleanAssertion"});
}

void passedNegativeBooleanAssertionShowsPassedMessage(TestResult &result) {
    assertEqual(result, "pass\n",
        {passesNegativeBooleanAssertion, "passesNegativeBooleanAssertion"});
}

void failedNegativeBooleanAssertionShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        "fail failsNegativeBooleanAssertion\n" +
            expectationMessage("false", "true"),
        {failsNegativeBooleanAssertion, "failsNegativeBooleanAssertion"});
}

void main() {
    testcpp::test(
        {{passedOnlyTestShowsPassedMessage, "passedOnlyTestShowsPassedMessage"},
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

int main() { testcpp::main(); }
