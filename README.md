# 🧠 Modern C++ Projects Portfolio

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)]()
[![C++](https://img.shields.io/badge/C%2B%2B-17/20-blue)]()
![Tests](https://img.shields.io/badge/tests-Catch2%20included-lightgrey)

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

**Goal:** Parse large log files efficiently using multithreading techniques and validate implementations via unit tests.

### 🧩 Concepts Practiced:

* `std::thread`, `std::mutex`, `std::async`, `std::future`
* Chunk-based and future-based parallelization strategies
* `constexpr`, `if constexpr`, and template metaprogramming
* RAII, scoped timing with `std::chrono`
* Use of modern testing frameworks (Catch2)

### 🎯 Features:

* Serial and two parallel implementations:
  * Thread-based with `std::thread` (via `LogParserParallelRunner`)
  * Future-based with `std::async` (via `LogParserParallelFuture`)
* Count `INFO`, `WARN`, `ERROR` messages
* Thread-safe parsing and aggregation
* Scalable benchmarking across file sizes and thread counts
* Real-time timing information and test validation
* Extensive Catch2-based test suite for functional and performance checks

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

## ✅ 5. Parallel Mandelbrot Renderer (Phase 5)

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

## ✅ 6. Neural Network from Scratch (Phase 6)

**Goal:** Implement a simple feedforward neural network (MLP) to classify data (e.g., XOR or MNIST subset) using only modern C++ and Eigen for matrix math.

### 🧩 Concepts Practiced:

* `Eigen::MatrixXd` for vectorized computation
* Backpropagation algorithm (from scratch)
* `std::function`, lambdas, and activation functions
* Minimal `SGD` (Stochastic Gradient Descent)
* Loss functions (MSE, CrossEntropy)

### 🎯 Features:

* Build multi-layer perceptrons with customizable topology
* Train on toy datasets like XOR or linearly separable data
* Output training loss, weights, and predictions
* Extendable to image data via CSV (e.g., MNIST subset)

### 🧪 Testing:

* Catch2 tests ensure correct forward pass, loss, and backpropagation updates

### 🔧 Build Instructions:

```bash
mkdir build && cd build
cmake ..
make
./NeuralNet
```


---

## ✅ 7. BVH Ray Tracer (Phase 7)

**Goal:** Build a modular ray tracing engine with scene factories, polymorphic geometry/material systems, BVH acceleration, OBJ mesh loading, and parallel rendering paths.

### 🧩 Concepts Practiced:

* Object-oriented design with polymorphic `hittable`, `material`, `texture`, and camera abstractions
* `std::unique_ptr` ownership and move semantics across scene construction and BVH assembly
* Recursive BVH construction and hierarchical spatial acceleration
* Scene factory / scenario setup design
* Separation of rendering, scene setup, input, parallelism, and output responsibilities
* Mesh and OBJ/MTL loading pipelines
* Parallel and distributed rendering structure (`MPI`, parallel camera / renderer variants)

### 🎯 Features:

* Multiple built-in scenes, including random spheres, Cornell box, lights, smoke, quads, and OBJ-based scenes
* BVH-based acceleration structure for faster ray-object intersection
* Support for spheres, quads, circles, triangles, meshes, and volumetric media
* Material system with Lambertian, metal, dielectric, light-emitting, and texture-based surfaces
* Texture support including checker, Perlin noise, and image textures
* Scene construction through a dedicated `scene_factory`
* OBJ/MTL model import for rendering external geometry
* Parallel rendering architecture with MPI-oriented and parallel camera / world components
* Output pipeline for writing rendered images to file
* Animated scene support through a dedicated renderer / scenario path

---

## ✅ 8. Concurrent Simulation Engine (Phase 8)

**Goal:** Develop a modular 3D particle simulation engine with extensible physics and parallel-ready architecture.

---

### 🧩 Concepts Practiced:

- Modern C++ (STL containers, smart pointers, RAII)
- Strategy Pattern (Integrator, ForceField, Neighbor models)
- Factory Pattern (EngineFactory command-based construction)
- Composite Pattern (FixList runtime hook system)
- Mediator-style central architecture (Engine)
- Unit & regression testing with Catch2

---

### 🎯 Features:

- Modular physics components (ForceField, Neighbor, Integrator)
- Runtime-extensible Fix system (init/setup/integrate hooks)
- Configurable simulation via command-driven factory
- Neighbor-based particle interactions
- Multiple integration strategies (Euler, Semi-implicit, Collision-aware)
- Structured simulation run loop
- Performance-conscious particle data layout
- End-to-end and unit-level validation tests


## 📂 Repository Structure

```
project-root/
│
├── TaskManager/
├── FinancialAnalyzer/
├── LogParser/
├── AsyncScheduler/
├── SimulationEngine/
├── Mandelbrot/
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
