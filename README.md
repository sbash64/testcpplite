# TestCppLite

## Purpose

provide a simple, lightweight testing library for C++ without inheritance or macros

## Build

```bash
$ cmake -S . -B build
$ cmake --build build --target sbash64-testcpplite
```

## Use

```c++
#include <sbash64/testcpplite/testcpplite.hpp>

#include <iostream>
#include <string>

using std::literals::string_literals::operator""s;

static void passes(sbash64::testcpplite::TestResult &result) {
    assertEqual(result, 1, 1);
}

static void fails(sbash64::testcpplite::TestResult &result) {
    assertEqual(result, "a"s, "b"s);
}

int main() {
    return sbash64::testcpplite::test(
        {
            {passes, "this test will pass"},
            {fails, "this test will fail"}
        },
        std::cout
    );
}
```

Output:

```
failed this test will fail
expected:
"a"
actual:
"b"
```
