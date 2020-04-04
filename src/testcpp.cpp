#include "testcpp.hpp"

namespace testcpp {
static bool failed_;

static void failed() { failed_ = true; }

struct TestResult {};

void test(const std::vector<Test> &tests, std::ostream &stream) {
    bool passed{true};
    for (const auto &test : tests) {
        TestResult result;
        test.f(result);
        if (failed_) {
            passed = false;
            stream << "fail " << test.name.c_str() << '\n';
        }
        failed_ = false;
    }
    if (passed)
        stream << "pass\n";
}

void assertEqual(
    TestResult &, const std::string &expected, const std::string &actual) {
    if (expected != actual)
        failed();
}

void assertEqual(TestResult &, int expected, int actual) {
    if (expected != actual)
        failed();
}

void assertTrue(TestResult &, bool c) {
    if (!c)
        failed();
}

void assertFalse(TestResult &, bool c) {
    if (c)
        failed();
}
}
