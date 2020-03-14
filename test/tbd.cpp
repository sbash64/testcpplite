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
int failed_;

void failed() {
    ++failed_;
    failedName_ = currentName_;
}

void test(const std::vector<Test> &tests, std::ostream &stream) {
    for (auto test : tests) {
        currentName_ = test.name;
        test.f();
    }
    stream << (failed_ == 0 ? "pass\n" : "fail " + failedName_ + '\n');
    failed_ = 0;
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
}
}

namespace testcpp {
void main() {
    test({{testPassedOnlyTestShowsPassedMessage,
              "testPassedOnlyTestShowsPassedMessage"},
             {testFailedOnlyTestShowsFailedMessage,
                 "testFailedOnlyTestShowsFailedMessage"}},
        std::cout);
}
}

int main() { testcpp::main(); }
