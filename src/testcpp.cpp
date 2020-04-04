#include "testcpp.hpp"

namespace testcpp {
static bool failed_;

static void failed() {
    failed_ = true;
}

void test(const std::vector<Test> &tests, std::ostream &stream) {
    bool passed{true};
    for (const auto &test : tests) {
        test.f();
        if (failed_) {
            passed = false;
            stream << "fail " << test.name.c_str() << '\n';
        }
        failed_ = false;
    }
    if (passed)
        stream << "pass\n";
}

void assertEqual(const std::string &expected, const std::string &actual) {
    if (expected != actual)
        failed();
}

void assertEqual(int expected, int actual) {
    if (expected != actual)
        failed();
}

void assertTrue(bool c) {
    if (!c)
        failed();
}

void assertFalse(bool c) {
    if (c)
        failed();
}
}
