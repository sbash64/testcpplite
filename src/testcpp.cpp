#include "testcpp.hpp"

namespace testcpp {
static const char *failedName;
static const char *currentName;
static bool failed_;

static void failed() {
    failed_ = true;
    failedName = currentName;
}

void test(const std::vector<Test> &tests, std::ostream &stream) {
    bool passed{true};
    for (const auto &test : tests) {
        const auto restoreName{currentName};
        currentName = test.name.c_str();
        test.f();
        if (failed_) {
            passed = false;
            stream << "fail " << failedName << '\n';
        }
        failed_ = false;
        currentName = restoreName;
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
