![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![License](https://img.shields.io/badge/license-MIT-blue)
![C++](https://img.shields.io/badge/language-C%2B%2B20-blue)
![Tests](https://img.shields.io/badge/tests-Catch2%20included-lightgrey)

# Log File Parser Benchmark Project

This project generates synthetic log files with various proportions of `[INFO]`, `[WARN]`, and `[ERROR]` messages, then benchmarks different strategies for parsing them:

- `LogParser`: serial parser.
- `LogParserThreads`: `std::thread`-based parser that splits the file into chunks.
- `LogParserFuture`: `std::async`-based parser that reads chunks in parallel.

## Features

- Synthetic log file generation with configurable message proportions and sizes.
- Parsing strategies supporting single-threaded and multi-threaded approaches.
- Performance benchmarks for different file sizes and thread configurations.

## Project Structure

- `CreateLargeLogs.h` / `CreateLargeLogs.cpp`: log file generator.
- `fileWriter.h`: file metadata and writer utilities used by the generator.
- `DataStructure.h` / `DataStructure.cpp`: shared container for counts and line storage.
- `LogParser.h` / `LogParser.cpp`: base serial parser and common parsing logic.
- `LogParserThreads.h` / `LogParserThreads.cpp`: thread-based parallel parser.
- `LogParserFuture.h` / `LogParserFuture.cpp`: future-based parallel parser.
- `test.hpp` / `test.cpp`: Catch2 tests and benchmarks.

## Test Framework

Unit tests and performance benchmarks use **[Catch2](https://github.com/catchorg/Catch2)** via the single-header amalgamation (`catch_amalgamated.hpp` / `catch_amalgamated.cpp`).

### How to Build and Run Tests

1. **Visual Studio**:
   - Open `LargeLogParser.vcxproj`.
   - Ensure the language standard is set to C++20 (required for `<format>`).
   - Build and run the test target that includes `test.cpp`.

2. **Command line (example)**:
   ```bash
   cl /std:c++20 /EHsc test.cpp CreateLargeLogs.cpp DataStructure.cpp LogParser.cpp LogParserThreads.cpp LogParserFuture.cpp catch_amalgamated.cpp
   ```

3. **Run Tests**:
   Execute the compiled test binary. Timing and validation output will be printed to the console.

## Test Coverage

The test suite benchmarks and verifies:
- Parser correctness (counts of `[INFO]`, `[WARN]`, `[ERROR]` lines).
- Scalability with increasing file sizes.
- Performance scaling with varying thread counts.
- Comparison between serial and parallel implementations.
