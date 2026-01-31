![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![License](https://img.shields.io/badge/license-MIT-blue)
![C++](https://img.shields.io/badge/language-C%2B%2B20-blue)
![Tests](https://img.shields.io/badge/tests-Catch2%20included-lightgrey)

# Log File Parser Benchmark Project

This project generates synthetic log files with configurable proportions of `[INFO]`, `[WARN]`, and `[ERROR]` messages, and benchmarks multiple parsing strategies:

- **`LogParser`**: serial parser.
- **`LogParserThreads`**: `std::thread`-based parser that splits the file into chunks.
- **`LogParserFuture`**: `std::async`-based parser that reads chunks in parallel.
- **Creators**: helper classes to build parser configurations / pipelines (`LogParserCreater*`).

---

## Features

- Synthetic log generation with configurable size and message mix.
- Multiple parsing implementations (serial + parallel).
- Benchmarks to compare scaling with file size and thread count.
- Cross-platform build via **CMake** (Linux + Windows).

---

## Project Layout

```
.
├── main.cpp
├── Data_NS/
│   ├── DataStructure.h
│   └── DataStructure.cpp
├── Parser_NS/
│   ├── LogParser.h / .cpp
│   ├── LogParserThreads.h / .cpp
│   ├── LogParserFuture.h / .cpp
│   ├── LogParserCreater.h
│   ├── LogParserCreaterConc.h / .cpp
│   └── LogParserCreaterLazy.h / .cpp
└── Test_NS/
    ├── test.hpp / test.cpp
    ├── CreateLargeLogs.h / .cpp
    ├── fileWriter.h
    └── catch_amalgamated.hpp / catch_amalgamated.cpp
```

---

## Build with CMake

### Targets

- **`logparser_app`**: builds `main.cpp` + core library.
- **`unit_tests`**: Catch2-based tests/benchmarks.

> If you keep app and tests in one Visual Studio project, you’ll fight “multiple `main()`” issues.  
> With CMake, you naturally get **two executables** (recommended).

---

## Windows (Visual Studio 2026 / MSVC)

From a Developer Command Prompt (or open the folder in Visual Studio and let it configure CMake):

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
ctest --test-dir build -C Release
```

Run the executables:

```bat
build\Release\logparser_app.exe
build\Release\unit_tests.exe
```

---

## Linux (Intel oneAPI `icpx`)

### Important note about `<format>`

On Ubuntu 22.04 the default GCC is often **11.x**, and **libstdc++ 11 does not provide `<format>`**.
If your sources include `<format>`, you must use a newer toolchain (recommended):

- **Option A (recommended): install GCC 13** and point `icpx` at it
- **Option B: use libc++** (requires libc++ packages and flags)

#### Option A — GCC 13 + `icpx`

```bash
# one-time: install newer GCC toolchain
sudo apt update
sudo apt install software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install gcc-13 g++-13
```

Configure/build with `icpx` using GCC 13’s libstdc++:

```bash
source /opt/intel/oneapi/setvars.sh

rm -rf build
cmake -S . -B build   -DCMAKE_CXX_COMPILER=icpx   -DCMAKE_C_COMPILER=icx   -DCMAKE_CXX_FLAGS="--gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/13"   -DCMAKE_BUILD_TYPE=Release

cmake --build build -j
ctest --test-dir build
```

#### Option B — libc++ (if you prefer)

```bash
sudo apt update
sudo apt install libc++-dev libc++abi-dev
```

Then configure with flags:

```bash
source /opt/intel/oneapi/setvars.sh

rm -rf build
cmake -S . -B build   -DCMAKE_CXX_COMPILER=icpx   -DCMAKE_C_COMPILER=icx   -DCMAKE_CXX_FLAGS="-stdlib=libc++ -isystem /usr/include/c++/v1"   -DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++"   -DCMAKE_BUILD_TYPE=Release

cmake --build build -j
ctest --test-dir build
```

---

## Tests

Tests and benchmarks are implemented with **Catch2** using the **amalgamated** distribution:

- `Test_NS/catch_amalgamated.hpp`
- `Test_NS/catch_amalgamated.cpp`

To run:

```bash
./build/unit_tests
```

---

## Notes

- If you see linker errors about missing/duplicate `main`, it usually means app and tests were linked into the same binary. With CMake, keep them as separate targets (`logparser_app`, `unit_tests`).
- If Linux compilation fails on `<format>`, it’s almost always a standard-library/toolchain issue (see Linux section above).

