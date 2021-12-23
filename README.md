# TestCppLite

## Purpose

provide a simple, lightweight testing library for C++ without inheritance or macros

## Build

```bash
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --target sbash64-testcpplite
```

## Use

```c++
#include <sbash64/testcpplite/testcpplite.hpp>

#include <iostream>
#include <string>

using std::literals::string_literals::operator""s;

namespace sbash64 {
namespace testcpplite {
static void passes(TestResult &result) {
    assertEqual(result, 1, 1);
}

static void fails(TestResult &result) {
    assertEqual(result, "a"s, "b"s);
}
}
}

int main() {
    return sbash64::testcpplite::test(
        {{passes, "this test will pass"}, {fails, "this test will fail"}},
        std::cout
    );
}
```

```bash
$ ./a.out
failed this test will fail
expected:
"a"
actual:
"b"
```
