# Modern C++ Projects

This repository contains a collection of C++ projects that progressively demonstrate modern C++ concepts, patterns, and concurrency techniques. Each project builds on the previous one to provide a comprehensive showcase of modern programming skills.

---

## ✅ 1. Modern Task Manager (Phase 1)

🎯 **Goal:** Build a CLI app that manages tasks with priorities and categories.

**Concepts Practiced:**

* `std::map`, `std::vector`, `std::unique_ptr`
* Lambda-based sorting and filtering
* RAII for resource management

**Features:**

* Add / Remove / Update tasks
* Sort by priority or due date
* Save and load tasks to/from a file
* View overdue or upcoming tasks
* Filter by high-priority tasks

---

## ✅ 2. Financial Data Analyzer (Phase 2)

📈 **Goal:** Read stock data from CSV files and compute statistics (mean, median, etc).

**Concepts Practiced:**

* `std::ifstream`, `std::stringstream`
* `std::transform`, `std::accumulate`, ranges
* Data aggregation and filtering

**Features:**

* Filter by date range or ticker
* Output statistics: average price, max drawdown, etc.
* Sort by volume or price

---

## ✅ 3. Multithreaded Log Parser (Phase 3)

🧵 **Goal:** Parse large log files in parallel and summarize errors, warnings, etc.

**Concepts Practiced:**

* `std::thread`, `std::mutex`, `std::future`, `std::async`
* Thread-safe containers
* Log parsing using string utilities

**Features:**

* Count lines with `ERROR`, `WARN`, etc.
* Multi-threaded parsing of file chunks
* Real-time progress reporting

---

## ✅ 4. Async Task Scheduler (Phase 4)

⏱️ **Goal:** Simulate asynchronous tasks like fake I/O or background work.

**Concepts Practiced:**

* C++20 coroutines: `co_await`, `co_return`
* `std::optional`, `std::variant`
* `std::future`, `std::async`

**Features:**

* Queue of fake I/O tasks
* Prioritized / delayed execution
* Cancellable tasks using `std::stop_token`

---

## ✅ 5. Concurrent Simulation Engine (Phase 5)

🚦 **Goal:** Simulate a grid of agents updating in parallel (e.g., particles or traffic).

**Concepts Practiced:**

* STL + concurrency + RAII
* Design patterns: Factory, Strategy using `std::variant`
* Logging and command-line configuration

**Features:**

* Each grid cell holds an agent or is empty
* Agents react to neighbors and decide next move
* Step-based or async simulation loop
* Performance and logging tools

---

## ✅ 6. Mandelbrot Set Visualizer (Phase 6)

🎯 **Goal:** Launch a high-performance missile into the heart of parallel computing by rendering the Mandelbrot set using **OpenMP**.

**Concepts Practiced:**

* Custom numerical types (complex number class)
* Header-based modular design (`array.h`, `definitions.h`)
* Separation of interface/implementation (`mandelbrot.h/.cpp`)
* **Parallel for-loops with OpenMP**
* Procedural image generation with numerical stability

**Features:**

* Renders Mandelbrot fractals using custom complex number arithmetic
* Efficient **multithreaded computation** via OpenMP (`#pragma omp parallel for`)
* User-definable grid bounds, resolution, and iteration depth
* Clean separation of computation logic and rendering interface
* Easily extendable to graphical display or GPU offloading

---

🚀 Each project in this repository is written with readability, extensibility, and performance in mind — ready to be showcased, built upon, or adapted into larger systems.

Feel free to fork, contribute, or get inspired.
