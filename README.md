# 🧠 Modern C++ Projects Portfolio

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)]()
[![C++](https://img.shields.io/badge/C%2B%2B-17/20-blue)]()

Welcome to my collection of modern C++ projects, crafted to demonstrate practical software engineering skills using STL, RAII, concurrency, and modular design. These projects progress in difficulty and complexity, and are all built with clean, testable, and extensible architecture in mind.

---

## ✅ 1. Modern Task Manager (Phase 1)

**Goal:** Build a CLI app that manages tasks with priorities and categories.

### 🧩 Concepts Practiced:

* `std::map`, `std::vector`, `std::unique_ptr`
* Lambda-based sorting and filtering
* RAII for file/resource cleanup

### 🎯 Features:

* Add/Remove/Update tasks
* Sort by priority, name, or due date
* Save/load tasks from file

### 🔧 Build Instructions:

```bash
mkdir build && cd build
cmake ..
make
./TaskManager
```

---

## ✅ 2. Financial Data Analyzer (Phase 2)

**Goal:** Read stock data from CSV and compute statistics (mean, median, etc.).

### 🧩 Concepts Practiced:

* `std::ifstream`, `std::stringstream`
* `std::transform`, `std::accumulate`, `ranges`
* Aggregation and filtering

### 🎯 Features:

* Filter by date or ticker
* Compute average price, max drawdown, etc.
* Sort by volume or price

---

## ✅ 3. Multithreaded Log Parser (Phase 3)

**Goal:** Parse large log files in parallel and summarize results.

### 🧩 Concepts Practiced:

* `std::thread`, `std::mutex`, `std::async`, `std::future`
* Thread-safe containers
* Regex or substring-based parsing

### 🎯 Features:

* Count ERROR, WARN lines
* Chunk-based parallel parsing
* Real-time progress reporting

---

## ✅ 4. Async Task Scheduler (Phase 4)

**Goal:** Simulate asynchronous I/O-like task execution.

### 🧩 Concepts Practiced:

* C++20 Coroutines (`co_await`, `co_return`)
* `std::optional`, `std::variant`
* `std::future`, cancellation with `stop_token`

### 🎯 Features:

* Queue of async tasks
* Task prioritization and delay
* Cancellation and progress monitoring

---

## ✅ 5. Concurrent Simulation Engine (Phase 5)

**Goal:** Simulate a 2D agent-based system (e.g., particles or traffic) in parallel.

### 🧩 Concepts Practiced:

* STL with concurrency support
* RAII + design patterns (Factory, Strategy)
* `std::variant` for agent polymorphism

### 🎯 Features:

* Agents interact with neighbors
* Step or async loop simulation
* Configurable parameters & performance metrics

---

## 🚀 6. Parallel Mandelbrot Renderer (Phase 6)

**Goal:** Render Mandelbrot fractals using OpenMP for parallelism.

### 🧩 Concepts Practiced:

* OpenMP parallel for pixel computation
* Manual complex number implementation
* Modular header design (`array`, `complex`, `definitions`)
* ** Testing the effect of memory layout on the openmp peformance

### 🎯 Features:

* Multi-threaded rendering engine
* Zoom/resolution tuning
* Output visualization via PPM or display API

---

## 📂 Repository Structure

```
project-root/
│
├── Phase1_TaskManager/
├── Phase2_FinancialAnalyzer/
├── Phase3_LogParser/
├── Phase4_AsyncScheduler/
├── Phase5_SimulationEngine/
├── Phase6_Mandelbrot/
└── README.md
```

## 📬 Contact

For questions, collaboration, or feedback:
**Mahdi Tavakol**
📧 [mahditavakol90@gmail.com](mailto:mahditavakol90@gmail.com)

---

## 🧪 Future Plans

* Unit tests for each module
* GUI frontend for some phases
* Cross-platform builds using CMake presets
* Optional Boost/Qt integration

> 🌟 If you found these projects useful or inspiring, feel free to ⭐ the repo and follow me for more C++ content!
