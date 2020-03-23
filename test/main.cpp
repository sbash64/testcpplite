#include <testcpp/testcpp.hpp>
#include <sstream>
#include <iostream>

namespace testcpp {
namespace {
void passes() { assertEqual("a", "a"); }

void fails() { assertEqual("a", "b"); }

void passesIntegerComparison() { assertEqual(1, 1); }

void failsIntegerComparison() { assertEqual(1, 0); }

void passesBooleanAssertion() { assertTrue(true); }

void failsBooleanAssertion() { assertTrue(false); }

auto test(const std::vector<Test> &tests) -> std::stringstream {
    std::stringstream stream;
    test(tests, stream);
    return stream;
}

void assertEqual(const std::string &s, const std::stringstream &stream) {
    std::cout << "expected: " << s << "actual: " << stream.str() << '\n';
    testcpp::assertEqual(s, stream.str());
}

void passedOnlyTestShowsPassedMessage() {
    assertEqual("pass\n", test({{passes, "passes"}}));
}

void failedOnlyTestShowsFailedMessage() {
    assertEqual("fail fails\n", test({{fails, "fails"}}));
}

void failedOneOfTwoTestsShowsFailedMessage() {
    assertEqual("fail fails\n", test({{passes, "passes"}, {fails, "fails"}}));
}

void failsBothTestsShowsFailedMessage() {
    assertEqual(
        "fail fail1\nfail fail2\n", test({{fails, "fail1"}, {fails, "fail2"}}));
}

void passesLastTestButFailsFirstShowsFailedMessage() {
    assertEqual("fail fails\n", test({{fails, "fails"}, {passes, "passes"}}));
}

void passedIntegerComparisonShowsPassedMessage() {
    assertEqual(
        "pass\n", test({{passesIntegerComparison, "integerComparisonPass"}}));
}

void failedIntegerComparisonShowsFailedMessage() {
    assertEqual("fail failsIntegerComparison\n",
        test({{failsIntegerComparison, "failsIntegerComparison"}}));
}

void passedBooleanAssertionShowsPassedMessage() {
    assertEqual(
        "pass\n", test({{passesBooleanAssertion, "passesBooleanAssertion"}}));
}

void failedBooleanAssertionShowsFailedMessage() {
    assertEqual("fail failsBooleanAssertion\n",
        test({{failsBooleanAssertion, "failsBooleanAssertion"}}));
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
                "failedBooleanAssertionShowsFailedMessage"}},
        std::cout);
}
}
}

int main() { testcpp::main(); }
