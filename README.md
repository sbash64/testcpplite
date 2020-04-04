# TestCppLite
## Purpose
* a simple, lightweight testing library for C++ without inheritance or macros.
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
    testcpplite::test({{passes, "myTest"}, {fails, "myFailingTest"}}, std::cout);
}
```

```bash
$ ./a.out
fail myFailingTest
     expected "a", actual "b"
```