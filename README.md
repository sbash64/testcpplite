# TestCppLite

## Purpose
provide a simple, lightweight testing library for C++ without inheritance or macros

## Build
```bash
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . --target testcpplite
```

## Use
```c++
#include <testcpplite/testcpplite.hpp>
#include <iostream>

static void passes(testcpplite::TestResult &result) {
    testcpplite::assertEqual(result, 1, 1);
}

static void fails(testcpplite::TestResult &result) {
    testcpplite::assertEqual(result, "a", "b");
}

int main() {
    return testcpplite::test(
        {{passes, "thisTestWillPass"}, {fails, "thisTestWillFail"}},
        std::cout
    );
}
```

```bash
$ ./a.out
fail thisTestWillFail
     expected "a", actual "b"
```