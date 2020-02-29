#include <string>
#include <vector>
#include <functional>
#include <ostream>

namespace testcpp {
int failed_;

void failed() { ++failed_; }

void test(
    const std::vector<std::function<void()>> &tests, std::ostream &stream) {
    for (auto f : tests)
        f();
    stream << (failed_ == 0 ? "pass\n" : "fail fails\n");
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

void testPassedOnlyTestShowsPassedMessage() {
    std::stringstream stream;
    test({passes}, stream);
    assertEqual("pass\n", stream.str());
}

void testFailedOnlyTestShowsFailedMessage() {
    std::stringstream stream;
    test({fails}, stream);
    assertEqual("fail fails\n", stream.str());
}
}
}

namespace testcpp {
void main() {
    test({testPassedOnlyTestShowsPassedMessage,
             testFailedOnlyTestShowsFailedMessage},
        std::cout);
}
}

int main() { testcpp::main(); }
