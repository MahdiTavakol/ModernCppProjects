# Modern C++ Project Series: From CLI to Concurrency 🚀

Welcome to a multi-phase project series designed to demonstrate practical, real-world applications of **Modern C++ (C++17/20)**. Each phase builds on the previous, introducing more advanced concepts—from basic task management to concurrent simulation engines.

This series is ideal for demonstrating **job-ready C++ skills**, including STL mastery, memory safety with smart pointers, modern idioms like lambdas and coroutines, and concurrent programming.

---

## ✅ Phase 1: Modern Task Manager

**Goal:**  
Build a CLI application to manage tasks with priorities and categories.

**Concepts Practiced:**
- `std::map`, `std::vector`, `std::unique_ptr`
- Lambda-based sorting and filtering
- RAII for file/resource cleanup

**Features:**
- Add, remove, and update tasks
- Sort by priority, due date, or name
- Save/load tasks from file
- Filter tasks by priority or deadline

---

## ✅ Phase 2: Financial Data Analyzer

**Goal:**  
Parse and analyze stock data from CSV files.

**Concepts Practiced:**
- `std::ifstream`, `std::stringstream`
- `std::transform`, `std::accumulate`, `ranges::views`
- Custom aggregation and filtering

**Features:**
- Filter by ticker or date range
- Compute average price, median, volatility, drawdown
- Sort by volume, price, or date

---

## ✅ Phase 3: Multithreaded Log Parser

**Goal:**  
Parse large log files in parallel and extract key metrics.

**Concepts Practiced:**
- `std::thread`, `std::mutex`, `std::async`, `std::future`
- Thread-safe containers, critical sections
- String parsing and pattern matching

**Features:**
- Count log levels (ERROR, WARN, INFO, etc.)
- Multi-threaded chunk parsing for large files
- Real-time progress reporting

---

## ✅ Phase 4: Async Task Scheduler

**Goal:**  
Simulate a system of asynchronous tasks (e.g., I/O fetches).

**Concepts Practiced:**
- Coroutines (`co_await`, `co_return`)
- `std::optional`, `std::variant` for task states
- `std::future`, `std::async`, `std::stop_token`

**Features:**
- Async execution queue
- Task prioritization and cancellation
- Simulated I/O delay and task completion

---

## ✅ Phase 5: Concurrent Simulation Engine

**Goal:**  
Build a grid-based simulation engine (e.g., traffic or agents).

**Concepts Practiced:**
- STL + concurrency (threads, async)
- RAII, strategy/factory design patterns
- Performance monitoring and logging

**Features:**
- Agents move and act based on neighborhood
- Configurable grid and behavior
- Synchronous and asynchronous stepping
- Benchmarking and profiling

---

## 🔧 Build & Run

Each phase lives in its own subfolder. Use `CMake` or your preferred compiler:

```bash
g++ -std=c++20 -pthread -o task_manager main.cpp Task.cpp TaskManager.cpp ChronoDate.cpp
./task_manager
