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

namespace sbash64 {
namespace testcpplite {
static void passes(TestResult &result) {
    assertEqual(result, 1, 1);
}

static void fails(TestResult &result) {
    assertEqual(result, "a", "b");
}
}
}

int main() {
    return sbash64::testcpplite::test(
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
