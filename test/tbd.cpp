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
    for (auto test : tests) {
        currentName_ = test.name;
        test.f();
        if (failed_) {
            ++failures;
            failureNames.push_back(failedName_);
        }
        failed_ = false;
    }
    if (failures == 0)
        stream << "pass\n";
    else
        for (auto name : failureNames)
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
    testcpp::assertEqual(s, stream.str());
}

void testPassedOnlyTestShowsPassedMessage() {
    std::stringstream stream;
    test({{passes, "passes"}}, stream);
    assertEqual("pass\n", stream);
}

void testFailedOnlyTestShowsFailedMessage() {
    std::stringstream stream;
    test({{fails, "fails"}}, stream);
    assertEqual("fail fails\n", stream);
}

void testFailedOneOfTwoTestsShowsFailedMessage() {
    std::stringstream stream;
    test({{passes, "passes"}, {fails, "fails"}}, stream);
    assertEqual("fail fails\n", stream);
}

void testFailsBothTestsShowsFailedMessage() {
    std::stringstream stream;
    test({{fails, "fail1"}, {fails, "fail2"}}, stream);
    assertEqual("fail fail1\nfail fail2\n", stream);
}
}
}

namespace testcpp {
void main() {
    test({{testPassedOnlyTestShowsPassedMessage,
              "testPassedOnlyTestShowsPassedMessage"},
             {testFailedOnlyTestShowsFailedMessage,
                 "testFailedOnlyTestShowsFailedMessage"},
             {testFailedOneOfTwoTestsShowsFailedMessage,
                 "testFailedOneOfTwoTestsShowsFailedMessage"},
             {testFailsBothTestsShowsFailedMessage,
                 "testFailsBothTestsShowsFailedMessage"}},
        std::cout);
}
}

int main() { testcpp::main(); }
