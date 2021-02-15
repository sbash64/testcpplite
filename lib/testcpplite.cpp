#include "testcpplite.hpp"
#include <exception>
#include <sstream>

namespace sbash64::testcpplite {
namespace {
struct QuotedString {
    std::string_view stringView;
};

struct FailedTestStream {
    std::ostream &stream;
    const Test &test;
    bool put{};
};
}

struct TestResult {
    FailedTestStream failureMessageStream;
    bool failed{};
};

static auto operator<<(std::ostream &stream, const QuotedString &s)
    -> std::ostream & {
    constexpr auto mark{'"'};
    return stream << mark << s.stringView << mark;
}

template <typename T>
static auto operator<<(FailedTestStream &stream, const T &s) -> std::ostream & {
    if (!stream.put) {
        stream.stream << "\x1b[31mfailed\x1b[0m " << stream.test.name;
        stream.put = true;
    }
    return stream.stream << s;
}

static void fail(TestResult &result) { result.failed = true; }

static auto putNewLine(std::ostream &stream) -> std::ostream & {
    return stream << '\n';
}

static auto putNewLine(FailedTestStream &stream) -> std::ostream & {
    return stream << '\n';
}

template <typename T>
void putExpectationMessage(
    TestResult &result, const T &expected, const T &actual) {
    putNewLine(
        putNewLine(putNewLine(putNewLine(putNewLine(result.failureMessageStream)
                                  << "expected:")
                       << expected)
            << "actual:")
        << actual);
}

static auto test(const Test &test, std::ostream &stream) -> bool {
    TestResult result{{stream, test}};
    try {
        test.f(result);
        if (!result.failed)
            return true;
    } catch (const std::exception &e) {
        putNewLine(putNewLine(result.failureMessageStream) << e.what());
    }
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
        putExpectationMessage(
            result, QuotedString{expected}, QuotedString{actual});
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
