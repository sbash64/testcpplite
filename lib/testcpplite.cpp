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

static auto putNewLine(std::ostream &stream) -> std::ostream & {
    return stream << '\n';
}

static auto test(const Test &test, std::ostream &stream) -> bool {
    std::stringstream failureStream;
    try {
        TestResult result{};
        test.f(result);
        if (!result.failed)
            return true;
        putNewLine(putNewLine(putNewLine(failureStream << "expected:")
                       << result.expected)
            << "actual:")
            << result.actual;
    } catch (const std::exception &e) {
        failureStream << e.what();
    }
    putNewLine(putNewLine(stream << "\x1b[31mfailed\x1b[0m " << test.name)
        << failureStream.str());
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
        setExpected(result, trueString);
        setActual(result, falseString);
        fail(result);
    }
}

void assertFalse(TestResult &result, bool c) {
    if (c) {
        setExpected(result, falseString);
        setActual(result, trueString);
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
