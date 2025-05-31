# Modern C++ Project Portfolio

This repository showcases a series of C++20/C++23 projects demonstrating modern C++ programming practices, including smart pointers, RAII, concurrency, coroutines, STL algorithms, and more. Each project is structured to highlight different advanced concepts, with real-world inspired use cases.

---

## ✅ 1. Modern Task Manager (Phase 1)

**Goal:** Build a CLI app that manages tasks with priorities and categories.

**Concepts Practiced:**

- `std::map`, `std::vector`, `std::unique_ptr`
- Lambda-based sorting and filtering
- RAII to manage file/resource cleanup

**Features:**

- Add/Remove/Update tasks
- Sort by priority or due date
- Save/load tasks to file
- Show completed, high-priority, and overdue tasks

---

## ✅ 2. Financial Data Analyzer (Phase 2)

**Goal:** Read stock data from CSV and compute statistics (mean, median, etc.).

**Concepts Practiced:**

- `std::ifstream`, `std::stringstream`
- `std::transform`, `std::accumulate`, `ranges::views`
- Aggregation and filtering

**Features:**

- Filter by date range or ticker
- Output statistics like average price, max drawdown
- Sort by volume or price

---

## ✅ 3. Multithreaded Log Parser (Phase 3)

**Goal:** Parse large log files in parallel and summarize results (e.g., error counts).

**Concepts Practiced:**

- `std::thread`, `std::mutex`, `std::async`, `std::future`
- Thread-safe containers or locks
- Log parsing using string utilities

**Features:**

- Count lines with ERROR, WARN, etc.
- Multi-threaded file chunk parsing
- Real-time progress reporting

---

## ✅ 4. Async Task Scheduler (Phase 4)

**Goal:** Simulate tasks that wait or work asynchronously (e.g., fetching URLs or files).

**Concepts Practiced:**

- Coroutines (`co_await`, `co_return`)
- `std::optional`, `std::variant` for task state
- `std::future`, `std::async` for fallback

**Features:**

- Queue of fake I/O tasks
- Prioritized or delayed execution
- Cancellable tasks using `stop_token` (C++20)

---

## ✅ 5. Concurrent Simulation Engine (Phase 5)

**Goal:** Simulate a grid of agents (e.g., traffic, particles) updating in parallel.

**Concepts Practiced:**

- STL + concurrency + modern C++ features
- RAII + design patterns (`Factory`, `Strategy` via `std::variant`)
- Logging, command-line config

**Features:**

- Each cell has an agent (or is empty)
- Agents decide next move based on neighbors
- Step-based or async simulation loop
- Performance metrics & structured logging

---

## ✅ 6. Mandelbrot Set Visualizer (Phase 6)

**Goal:** Generate and visualize the Mandelbrot set using a custom complex number class and modular design.

**Concepts Practiced:**

- Custom math types (`complex`)
- Header-based utilities (`array.h`, `definitions.h`)
- Separation of interface and implementation (`mandelbrot.h/.cpp`)
- Procedural image generation
- Use of modern C++ features like RAII

**Features:**

- Generates Mandelbrot fractals using complex number arithmetic  
- Flexible threading configuration support (`thread_config`)
- User-definable resolution and bounds  
- Modular file structure and extensibility for graphics or GPU acceleration

---

## 🔧 Build Instructions

Each project is self-contained in its own folder. You can build any project using:

```bash
g++ -std=c++20 -O2 -o app main.cpp [other .cpp files]
./app
