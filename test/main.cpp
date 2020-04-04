#include <testcpp/testcpp.hpp>
#include <sstream>
#include <iostream>

namespace testcpp {
namespace {
void passes(TestResult &result) { assertEqual(result, "a", "a"); }

void expectsAActualB(TestResult &result) { assertEqual(result, "a", "b"); }

void passesIntegerComparison(TestResult &result) { assertEqual(result, 1, 1); }

void expectsOneActualZero(TestResult &result) { assertEqual(result, 1, 0); }

void passesBooleanAssertion(TestResult &result) { assertTrue(result, true); }

void expectsTrueActualFalse(TestResult &result) { assertTrue(result, false); }

void passesNegativeBooleanAssertion(TestResult &result) {
    assertFalse(result, false);
}

void expectsFalseActualTrue(TestResult &result) { assertFalse(result, true); }

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

auto failMessage(const std::string &name) -> std::string {
    return "fail " + name + '\n';
}

auto failsExpectsAActualBMessage(const std::string &name) -> std::string {
    return failMessage(name) + expectationMessage("\"a\"", "\"b\"");
}

void passedOnlyTestShowsPassedMessage(TestResult &result) {
    assertEqual(result, "pass\n", {passes, "passes"});
}

void failedOnlyTestShowsFailedMessage(TestResult &result) {
    assertEqual(result, failsExpectsAActualBMessage("expectsAActualB"),
        {expectsAActualB, "expectsAActualB"});
}

void failedOneOfTwoTestsShowsFailedMessage(TestResult &result) {
    assertEqual(result, failsExpectsAActualBMessage("expectsAActualB"),
        {{passes, "passes"}, {expectsAActualB, "expectsAActualB"}});
}

void failsBothTestsShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        failsExpectsAActualBMessage("expectsAActualB1") +
            failsExpectsAActualBMessage("expectsAActualB2"),
        {{expectsAActualB, "expectsAActualB1"},
            {expectsAActualB, "expectsAActualB2"}});
}

void passesLastTestButFailsFirstShowsFailedMessage(TestResult &result) {
    assertEqual(result, failsExpectsAActualBMessage("expectsAActualB"),
        {{expectsAActualB, "expectsAActualB"}, {passes, "passes"}});
}

void passedIntegerComparisonShowsPassedMessage(TestResult &result) {
    assertEqual(
        result, "pass\n", {passesIntegerComparison, "passesIntegerComparison"});
}

void failedIntegerComparisonShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        failMessage("expectsOneActualZero") + expectationMessage("1", "0"),
        {expectsOneActualZero, "expectsOneActualZero"});
}

void passedBooleanAssertionShowsPassedMessage(TestResult &result) {
    assertEqual(
        result, "pass\n", {passesBooleanAssertion, "passesBooleanAssertion"});
}

void failedBooleanAssertionShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        failMessage("expectsTrueActualFalse") +
            expectationMessage("true", "false"),
        {expectsTrueActualFalse, "expectsTrueActualFalse"});
}

void passedNegativeBooleanAssertionShowsPassedMessage(TestResult &result) {
    assertEqual(result, "pass\n",
        {passesNegativeBooleanAssertion, "passesNegativeBooleanAssertion"});
}

void failedNegativeBooleanAssertionShowsFailedMessage(TestResult &result) {
    assertEqual(result,
        failMessage("expectsFalseActualTrue") +
            expectationMessage("false", "true"),
        {expectsFalseActualTrue, "expectsFalseActualTrue"});
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
