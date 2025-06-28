🧾 Log File Parser Benchmark Project
This project generates synthetic log files with various proportions of [INFO], [WARN], and [ERROR] messages, and benchmarks different strategies for parsing them:

LogParser: A serial log parser.

LogParserParallel: A parallel thread-based parser.

LogParserParallelFuture: A std::future-based parallel parser.

LogParserParallelRunner: A manager for parallel parsing using multiple threads.

🛠 Features
Synthetic log file generation with configurable message proportions and sizes.

Parsing strategies supporting single-threaded and multi-threaded approaches.

Performance benchmarks for different file sizes and thread configurations.

Thread-safe I/O and progress reporting for parallel parsing.

🧪 Test Framework
Unit tests and performance benchmarks are implemented using Catch2.

✔️ How to Build & Run Tests
Catch2 Integration:
The project uses a single-header version of Catch2 (catch_amalgamated.hpp). No separate installation is required.

Build with CMake or Visual Studio (Make sure you have C++20 enabled):

If using Visual Studio:

Add test.cpp to your test target or create a new test project and include all .h and .cpp files.

Make sure to define CATCH_CONFIG_MAIN in one .cpp file to enable the main test runner.

If using CMake:

cmake
Copy
Edit
add_executable(runTests test.cpp)
target_compile_features(runTests PRIVATE cxx_std_20)
Run Tests:
Run the compiled test binary in terminal or from within Visual Studio. All tests will run and show timing/validation output.

🧪 Test Coverage
The test suite benchmarks and verifies:

Parser correctness (e.g., count of [INFO], [WARN], [ERROR] lines).

Scalability with increasing file sizes.

Performance scaling with varying thread counts.

Comparison between serial and different parallel implementations.

Benchmark results are printed to the console in a structured format.
