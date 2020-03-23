#include <testcpp/testcpp.hpp>
#include <sstream>
#include <iostream>

namespace testcpp {
namespace {
void passes() { assertEqual("a", "a"); }

void fails() { assertEqual("a", "b"); }

void assertEqual(const std::string &s, const std::stringstream &stream) {
    std::cout << "expected: " << s << "actual: " << stream.str() << '\n';
    testcpp::assertEqual(s, stream.str());
}

void passedOnlyTestShowsPassedMessage() {
    std::stringstream stream;
    test({{passes, "passes"}}, stream);
    assertEqual("pass\n", stream);
}

void failedOnlyTestShowsFailedMessage() {
    std::stringstream stream;
    test({{fails, "fails"}}, stream);
    assertEqual("fail fails\n", stream);
}

void failedOneOfTwoTestsShowsFailedMessage() {
    std::stringstream stream;
    test({{passes, "passes"}, {fails, "fails"}}, stream);
    assertEqual("fail fails\n", stream);
}

void failsBothTestsShowsFailedMessage() {
    std::stringstream stream;
    test({{fails, "fail1"}, {fails, "fail2"}}, stream);
    assertEqual("fail fail1\nfail fail2\n", stream);
}

void main() {
    test(
        {{passedOnlyTestShowsPassedMessage, "passedOnlyTestShowsPassedMessage"},
            {failedOnlyTestShowsFailedMessage,
                "failedOnlyTestShowsFailedMessage"},
            {failedOneOfTwoTestsShowsFailedMessage,
                "failedOneOfTwoTestsShowsFailedMessage"},
            {failsBothTestsShowsFailedMessage,
                "failsBothTestsShowsFailedMessage"}},
        std::cout);
}
}
}

int main() { testcpp::main(); }
