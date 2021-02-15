#include "testcpplite.hpp"
#include <exception>
#include <sstream>

namespace sbash64::testcpplite {
namespace {
struct QuotedString {
    std::string_view stringView;
};
}

struct TestResult {
    std::ostream &stream;
    const Test &test;
    bool failed{};
};

static auto putColor(std::ostream &stream, std::string_view s,
    std::string_view code) -> std::ostream & {
    return stream << "\x1b[" << code << 'm' << s << "\x1b[0m";
}

static auto operator<<(std::ostream &stream, const QuotedString &s)
    -> std::ostream & {
    constexpr auto mark{'"'};
    return stream << mark << s.stringView << mark;
}

template <typename T>
static auto operator<<(TestResult &result, const T &s) -> std::ostream & {
    if (!result.failed) {
        result.failed = true;
        putColor(result.stream, "failed", "31") << ' ' << result.test.name;
    }
    return result.stream << s;
}

static auto putNewLine(std::ostream &stream) -> std::ostream & {
    return stream << '\n';
}

static auto putNewLine(TestResult &result) -> std::ostream & {
    return result << '\n';
}

template <typename T>
void putExpectationMessage(
    TestResult &result, const T &expected, const T &actual) {
    putNewLine(
        putNewLine(putNewLine(putNewLine(putNewLine(result) << "expected:")
                       << expected)
            << "actual:")
        << actual);
}

static auto test(const Test &test, std::ostream &stream) -> bool {
    TestResult result{stream, test};
    try {
        test.f(result);
    } catch (const std::exception &e) {
        putNewLine(putNewLine(result) << e.what());
    }
    return !result.failed;
}

auto test(const std::vector<Test> &tests, std::ostream &stream) -> int {
    bool passed{true};
    for (const auto &t : tests)
        passed &= test(t, stream);
    if (passed) {
        putColor(stream, "passed", "32") << " - " << tests.size() << " test";
        if (tests.size() != 1)
            stream << 's';
        putNewLine(stream);
        return 0;
    }
    return 1;
}

void assertEqual(
    TestResult &result, std::string_view expected, std::string_view actual) {
    if (expected != actual)
        putExpectationMessage(
            result, QuotedString{expected}, QuotedString{actual});
}

template <typename T>
void assertEqual(TestResult &result, T expected, T actual) {
    if (expected != actual)
        putExpectationMessage(result, expected, actual);
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
    if (!c)
        putExpectationMessage(result, trueString, falseString);
}

void assertFalse(TestResult &result, bool c) {
    if (c)
        putExpectationMessage(result, falseString, trueString);
}

void assertEqual(TestResult &result, const void *expected, const void *actual) {
    if (expected != actual)
        putExpectationMessage(result, expected, actual);
}
}
