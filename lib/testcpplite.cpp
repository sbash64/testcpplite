#include "testcpplite.hpp"
#include <exception>
#include <sstream>

namespace sbash64::testcpplite {
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

static auto quoted(std::string_view s) -> std::string {
    std::stringstream stream;
    constexpr auto mark{'"'};
    stream << mark << s << mark;
    return stream.str();
}

static void writeFailure(
    std::ostream &stream, const Test &test, const std::string &what) {
    stream << "fail " << test.name << '\n';
    stream << "    " << what << '\n';
}

static auto test(const Test &test, std::ostream &stream) -> bool {
    bool passed{true};
    try {
        TestResult result{};
        test.f(result);
        if (result.failed) {
            passed = false;
            writeFailure(stream, test,
                "expected " + result.expected + ", actual " + result.actual);
        }
    } catch (const std::exception &e) {
        passed = false;
        writeFailure(stream, test, e.what());
    }
    return passed;
}

auto test(const std::vector<Test> &tests, std::ostream &stream) -> int {
    bool passed{true};
    for (const auto &t : tests)
        passed &= test(t, stream);
    if (passed)
        stream << "pass\n";
    return passed ? 0 : 1;
}

void assertEqual(
    TestResult &result, std::string_view expected, std::string_view actual) {
    if (expected != actual) {
        setExpected(result, quoted(expected));
        setActual(result, quoted(actual));
        fail(result);
    }
}

template <typename T>
void assertEqual(TestResult &result, T expected, T actual) {
    if (expected != actual) {
        setExpected(result, std::to_string(expected));
        setActual(result, std::to_string(actual));
        fail(result);
    }
}

void assertEqual(TestResult &result, int expected, int actual) {
    assertEqual<int>(result, expected, actual);
}

void assertEqual(TestResult &result, long expected, long actual) {
    assertEqual<long>(result, expected, actual);
}

void assertEqual(
    TestResult &result, unsigned long expected, unsigned long actual) {
    assertEqual<unsigned long>(result, expected, actual);
}

void assertEqual(TestResult &result, unsigned long long expected,
    unsigned long long actual) {
    assertEqual<unsigned long long>(result, expected, actual);
}

void assertEqual(TestResult &result, long long expected, long long actual) {
    assertEqual<long long>(result, expected, actual);
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

void assertEqual(TestResult &result, const void *expected, const void *actual) {
    if (expected != actual) {
        std::stringstream expectedStream;
        expectedStream << expected;
        std::stringstream actualStream;
        actualStream << actual;
        setExpected(result, expectedStream.str());
        setActual(result, actualStream.str());
        fail(result);
    }
}
}
