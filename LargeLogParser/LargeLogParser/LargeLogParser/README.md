# 🧵 Multithreaded Log Parser (Phase 3)

## 📌 Goal
Efficiently parse large log files in **parallel** to count occurrences of specific log levels like `[ERROR]`, `[WARN]`, and `[INFO]`.

---

## 🧩 Concepts Practiced

- `std::thread`, `std::mutex`, `std::async`, `std::future`
- Thread-safe logging
- Chunk-based file I/O
- Optional real-time progress reporting
- Optional regex-based pattern search

---

## 🎯 Features

- ✅ Count lines starting with `[ERROR]`, `[WARN]`, `[INFO]`
- ✅ Parallel parsing using multiple threads
- ✅ File is divided into non-overlapping byte chunks
- ✅ Each thread opens and processes its own chunk
- ✅ Skips partial line at the start of each chunk (if needed)
- ✅ Real-time progress display per thread

---

## 🏗️ Project Structure

```
.
├── LogParser.h              # Abstract base class with common utilities
├── LogParser.cpp
├── LogParserParallel.h     # Inherits from LogParser for multithreaded chunk parsing
├── LogParserRunner.h       # Manages threads, launches LogParserParallel instances
├── LogParserRunner.cpp
```

---

## 🧪 Sample Output (Console)

```
[Thread 0] Progress: 32.57%
[Thread 1] Progress: 47.19%
[Thread 2] Progress: 68.14%
[Thread 3] Progress: 90.83%
```

---

## 🧵 How It Works

1. `LogParserRunner` reads file size and splits it into N chunks.
2. For each chunk:
    - A new `LogParserParallel` is created
    - Each opens the file, seeks to its own `beg` and `end` position
    - First line skipped if `beg` is not at a newline
    - Each line is checked for `[ERROR]`, `[WARN]`, `[INFO]`
3. Threads join and results are aggregated

---

## 🔧 Notes

- File I/O is **not shared** between threads — each opens its own `ifstream`
- Progress is calculated using `tellg()` against chunk range
- Future enhancements can include:
  - Regex pattern matching
  - Configurable log levels
  - Output to JSON or summary report

---

## 📤 How to Run

```bash
g++ -std=c++20 -pthread -o parser main.cpp LogParser*.cpp
./parser logfile.txt 4
```
Arguments: `<file> <num_threads>`

---

## 👤 Author
Created by Mahdi Tavakol, 2025
