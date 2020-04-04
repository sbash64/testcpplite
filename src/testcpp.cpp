#include "testcpp.hpp"

namespace testcpp {
struct TestResult {
    std::string expected;
    std::string actual;
    bool failed;
};

static void fail(TestResult &result) { result.failed = true; }

void test(const std::vector<Test> &tests, std::ostream &stream) {
    bool passed{true};
    for (const auto &test : tests) {
        TestResult result{};
        test.f(result);
        if (result.failed) {
            passed = false;
            stream << "fail " << test.name.c_str() << '\n';
            stream << "    expected " << result.expected << ", actual "
                   << result.actual << "\n";
        }
    }
    if (passed)
        stream << "pass\n";
}

void assertEqual(TestResult &result, const std::string &expected,
    const std::string &actual) {
    if (expected != actual) {
        result.expected = '"' + expected + '"';
        result.actual = '"' + actual + '"';
        fail(result);
    }
}

void assertEqual(TestResult &result, int expected, int actual) {
    if (expected != actual) {
        result.expected = std::to_string(expected);
        result.actual = std::to_string(actual);
        fail(result);
    }
}

void assertTrue(TestResult &result, bool c) {
    if (!c) {
        result.expected = "true";
        result.actual = "false";
        fail(result);
    }
}

void assertFalse(TestResult &result, bool c) {
    if (c) {
        result.expected = "false";
        result.actual = "true";
        fail(result);
    }
}
}
