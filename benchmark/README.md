# Benchmark

If you want to do benchmarks of some iroha components, put them in this folder.

We use [google benchmark](https://github.com/google/benchmark).

To enable build of benchmarks, specify an option `-DBENCHMARKING=ON` during configure step:

```
$ cmake -DBENCHMARKING=ON ..
```

**Note**: changing `CMAKE_BUILD_TYPE` from `Debug` to `Release` can significantly speedup execution, thus heavily changing benchmark results.

To build iroha and benchmarks in release mode, type:
```
$ cmake  -DBENCHMARKING=ON -DCMAKE_BUILD_TYPE=Release ..
```

**Note**: all code inside benchmark function influences on the benchmark results.


# How to write benchmark

1. Include `#include <benchmark/benchmark.h>`.

2. Refer to official [google guilde](https://github.com/google/benchmark#example-usage).

Also, try to preserve namespacing by making directories inside `benchmark` directory.
