#include "testcpplite.hpp"
#include <exception>
#include <sstream>

namespace sbash64::testcpplite {
struct TestResult {
    std::stringstream failureStream;
    bool failed;
};

static void fail(TestResult &result) { result.failed = true; }

static auto quoted(std::string_view s) -> std::string {
    std::stringstream stream;
    constexpr auto mark{'"'};
    stream << mark << s << mark;
    return stream.str();
}

static auto putNewLine(std::ostream &stream) -> std::ostream & {
    return stream << '\n';
}

template <typename T>
void putExpectationMessage(
    TestResult &result, const T &expected, const T &actual) {
    putNewLine(
        putNewLine(putNewLine(result.failureStream << "expected:") << expected)
        << "actual:")
        << actual;
}

static auto test(const Test &test, std::ostream &stream) -> bool {
    TestResult result{};
    try {
        test.f(result);
        if (!result.failed)
            return true;
    } catch (const std::exception &e) {
        result.failureStream << e.what();
    }
    putNewLine(putNewLine(stream << "\x1b[31mfailed\x1b[0m " << test.name)
        << result.failureStream.str());
    return false;
}

auto test(const std::vector<Test> &tests, std::ostream &stream) -> int {
    bool passed{true};
    for (const auto &t : tests)
        passed &= test(t, stream);
    if (passed) {
        stream << "\x1b[32mpassed\x1b[0m - " << tests.size() << " test";
        if (tests.size() != 1)
            stream << 's';
        putNewLine(stream);
        return 0;
    }
    return 1;
}

void assertEqual(
    TestResult &result, std::string_view expected, std::string_view actual) {
    if (expected != actual) {
        fail(result);
        putExpectationMessage(result, quoted(expected), quoted(actual));
    }
}

template <typename T>
void assertEqual(TestResult &result, T expected, T actual) {
    if (expected != actual) {
        fail(result);
        putExpectationMessage(result, expected, actual);
    }
}

void assertEqual(TestResult &result, int expected, int actual) {
    assertEqual<>(result, expected, actual);
}

void assertEqual(TestResult &result, long expected, long actual) {
    assertEqual<>(result, expected, actual);
}

void assertEqual(
    TestResult &result, unsigned long expected, unsigned long actual) {
    assertEqual<>(result, expected, actual);
}

void assertEqual(TestResult &result, unsigned long long expected,
    unsigned long long actual) {
    assertEqual<>(result, expected, actual);
}

void assertEqual(TestResult &result, long long expected, long long actual) {
    assertEqual<>(result, expected, actual);
}

constexpr auto trueString{"true"};
constexpr auto falseString{"false"};

void assertTrue(TestResult &result, bool c) {
    if (!c) {
        fail(result);
        putExpectationMessage(result, trueString, falseString);
    }
}

void assertFalse(TestResult &result, bool c) {
    if (c) {
        fail(result);
        putExpectationMessage(result, falseString, trueString);
    }
}

void assertEqual(TestResult &result, const void *expected, const void *actual) {
    if (expected != actual) {
        fail(result);
        putExpectationMessage(result, expected, actual);
    }
}
}
