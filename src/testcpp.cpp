#include "testcpp.hpp"

namespace testcpp {
static std::string failedName_;
static std::string currentName_;
static bool failed_;

static void failed() {
    failed_ = true;
    failedName_ = currentName_;
}

void test(const std::vector<Test> &tests, std::ostream &stream) {
    std::vector<std::string> failureNames;
    for (const auto &test : tests) {
        auto restoreName{currentName_};
        currentName_ = test.name;
        test.f();
        if (failed_)
            failureNames.push_back(failedName_);
        failed_ = false;
        currentName_ = restoreName;
    }
    if (failureNames.empty())
        stream << "pass\n";
    else
        for (const auto &name : failureNames)
            stream << "fail " << name << '\n';
}

void assertEqual(const std::string &expected, const std::string &actual) {
    if (expected != actual)
        failed();
}
}