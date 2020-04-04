#include "testcpplite.hpp"

namespace testcpplite {
struct TestResult {
    std::string expected;
    std::string actual;
    bool failed;
};

static void setExpected(TestResult &result, std::string s) {
    result.expected = std::move(s);
}

static void setActual(TestResult &result, std::string s) {
    result.actual = std::move(s);
}

static void fail(TestResult &result) { result.failed = true; }

static auto quoted(const std::string &s) -> std::string {
    constexpr auto mark{'"'};
    return mark + s + mark;
}

auto test(const std::vector<Test> &tests, std::ostream &stream) -> int {
    bool passed{true};
    for (const auto &test : tests) {
        TestResult result{};
        test.f(result);
        if (result.failed) {
            passed = false;
            stream << "fail " << test.name << '\n';
            stream << "    expected " << result.expected << ", actual "
                   << result.actual << '\n';
        }
    }
    if (passed)
        stream << "pass\n";
    return passed ? 0 : 1;
}

void assertEqual(TestResult &result, const std::string &expected,
    const std::string &actual) {
    if (expected != actual) {
        setExpected(result, quoted(expected));
        setActual(result, quoted(actual));
        fail(result);
    }
}

void assertEqual(TestResult &result, int expected, int actual) {
    if (expected != actual) {
        setExpected(result, std::to_string(expected));
        setActual(result, std::to_string(actual));
        fail(result);
    }
}

void assertTrue(TestResult &result, bool c) {
    if (!c) {
        setExpected(result, "true");
        setActual(result, "false");
        fail(result);
    }
}

void assertFalse(TestResult &result, bool c) {
    if (c) {
        setExpected(result, "false");
        setActual(result, "true");
        fail(result);
    }
}
}
