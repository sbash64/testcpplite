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

void passesNegativeBooleanAssertion() { assertFalse(false); }

void failsNegativeBooleanAssertion() { assertFalse(true); }

auto test(const std::vector<Test> &tests) -> std::stringstream {
    std::stringstream stream;
    test(tests, stream);
    return stream;
}

void assertEqual(const std::string &s, const std::stringstream &stream) {
    std::cout << "expected: " << s << "actual: " << stream.str() << '\n';
    testcpp::assertEqual(s, stream.str());
}

void assertEqual(const std::string &s, const std::vector<Test> &tests) {
    assertEqual(s, test(tests));
}

void passedOnlyTestShowsPassedMessage() {
    assertEqual("pass\n", {{passes, "passes"}});
}

void failedOnlyTestShowsFailedMessage() {
    assertEqual("fail fails\n", {{fails, "fails"}});
}

void failedOneOfTwoTestsShowsFailedMessage() {
    assertEqual("fail fails\n", {{passes, "passes"}, {fails, "fails"}});
}

void failsBothTestsShowsFailedMessage() {
    assertEqual(
        "fail fail1\nfail fail2\n", {{fails, "fail1"}, {fails, "fail2"}});
}

void passesLastTestButFailsFirstShowsFailedMessage() {
    assertEqual("fail fails\n", {{fails, "fails"}, {passes, "passes"}});
}

void passedIntegerComparisonShowsPassedMessage() {
    assertEqual("pass\n", {{passesIntegerComparison, "integerComparisonPass"}});
}

void failedIntegerComparisonShowsFailedMessage() {
    assertEqual("fail failsIntegerComparison\n",
        {{failsIntegerComparison, "failsIntegerComparison"}});
}

void passedBooleanAssertionShowsPassedMessage() {
    assertEqual("pass\n", {{passesBooleanAssertion, "passesBooleanAssertion"}});
}

void failedBooleanAssertionShowsFailedMessage() {
    assertEqual("fail failsBooleanAssertion\n",
        {{failsBooleanAssertion, "failsBooleanAssertion"}});
}

void passedNegativeBooleanAssertionShowsPassedMessage() {
    assertEqual("pass\n",
        {{passesNegativeBooleanAssertion, "passesNegativeBooleanAssertion"}});
}

void failedNegativeBooleanAssertionShowsFailedMessage() {
    assertEqual("fail failsNegativeBooleanAssertion\n",
        {{failsNegativeBooleanAssertion, "failsNegativeBooleanAssertion"}});
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
