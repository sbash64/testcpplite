#include <string>
#include <vector>
#include <functional>
#include <ostream>

namespace testcpp {
struct Test {
    std::function<void()> f;
    std::string name;
};

std::string failedName_;
std::string currentName_;
bool failed_;

void failed() {
    failed_ = true;
    failedName_ = currentName_;
}

void test(const std::vector<Test> &tests, std::ostream &stream) {
    int failures{};
    std::vector<std::string> failureNames;
    for (const auto &test : tests) {
        auto restoreName{currentName_};
        currentName_ = test.name;
        test.f();
        if (failed_) {
            ++failures;
            failureNames.push_back(failedName_);
        }
        failed_ = false;
        currentName_ = restoreName;
    }
    if (failures == 0)
        stream << "pass\n";
    else
        for (const auto &name : failureNames)
            stream << "fail " << name << '\n';
}

void assertEqual(const std::string &expected, const std::string &actual) {
    if (expected != actual)
        failed();
}
}

#include <sstream>
#include <iostream>

namespace testcpp {
namespace {
void passes() {}

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
}
}

namespace testcpp {
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

int main() { testcpp::main(); }
