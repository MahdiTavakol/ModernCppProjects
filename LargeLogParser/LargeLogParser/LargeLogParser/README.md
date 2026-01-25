![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![License](https://img.shields.io/badge/license-MIT-blue)
![C++](https://img.shields.io/badge/language-C%2B%2B17-blue)
![Tests](https://img.shields.io/badge/tests-Catch2%20included-lightgrey)

---

# 🧾 Log File Parser Benchmark Project

This project generates **synthetic log files** containing configurable proportions of `[INFO]`, `[WARN]`, and `[ERROR]` messages, and benchmarks multiple strategies for parsing them. The goal is to compare **correctness, scalability, and performance** across serial and parallel implementations.

The following parser implementations are provided:

- `LogParser` — **Serial** log parser.
- `LogParserParallel` — **Thread-based parallel** parser.
- `LogParserParallelFuture` — **`std::future`-based parallel** parser.
- `LogParserParallelRunner` — A coordinator for parallel parsing across multiple threads.

---

## 🛠 Features

- Synthetic log file generation with configurable message distributions and file sizes.
- Multiple parsing strategies, including single-threaded and multi-threaded approaches.
- Performance benchmarking across different file sizes and thread counts.
- Thread-safe I/O and structured progress reporting for parallel execution.

---

## 🧪 Test Framework

Unit tests and performance benchmarks are implemented using **[Catch2](https://github.com/catchorg/Catch2)**.

### ✔️ How to Build & Run Tests

1. **Catch2 Integration**  
   The project uses the single-header distribution of Catch2 (`catch_amalgamated.hpp`), so no external installation is required.

2. **Build with CMake or Visual Studio** (C++20 required):
   - **Visual Studio**:
     - Add `test.cpp` to your test target, or create a dedicated test project and include all relevant `.h` and `.cpp` files.
     - Ensure that `CATCH_CONFIG_MAIN` is defined in exactly one `.cpp` file to provide the Catch2 test runner.
   - **CMake**:
     ```cmake
     add_executable(runTests test.cpp)
     target_compile_features(runTests PRIVATE cxx_std_20)
     ```

3. **Run Tests**  
   Execute the compiled test binary from the command line or directly within Visual Studio. All correctness checks and benchmark timings will be printed to the console.

---

## 🧪 Test Coverage

The test suite validates and benchmarks:

- Parser correctness (accurate counts of `[INFO]`, `[WARN]`, and `[ERROR]` entries).
- Scalability with increasing log file sizes.
- Performance scaling with varying thread counts.
- Direct performance comparisons between serial and parallel implementations.

Benchmark results are reported to the console in a structured, human-readable format.
