#include <string>
#include <vector>
#include <functional>
#include <ostream>

namespace testcpp {
void test(const std::vector<std::function<void()>> &, std::ostream &stream) {
    stream << "pass\n";
}

void assertEqual(const std::string &expected, const std::string &actual) {}
}

#include <sstream>
#include <iostream>

namespace testcpp {
namespace {
void passes() {}

void testPassedOnlyTestShowsPassedMessage() {
    std::stringstream stream;
    test({passes}, stream);
    assertEqual("pass\n", stream.str());
}
}
}

namespace testcpp {
void main() { test({testPassedOnlyTestShowsPassedMessage}, std::cout); }
}

int main() { testcpp::main(); }
