#ifndef TESTCPP_SRC_INCLUDE_TESTCPP_TESTCPP_HPP_
#define TESTCPP_SRC_INCLUDE_TESTCPP_TESTCPP_HPP_

#include <string>
#include <vector>
#include <functional>
#include <ostream>

namespace testcpp {
struct Test {
    std::function<void()> f;
    std::string name;
};

void test(const std::vector<Test> &tests, std::ostream &stream);
void assertEqual(const std::string &expected, const std::string &actual);
void assertEqual(int expected, int actual);
}

#endif
