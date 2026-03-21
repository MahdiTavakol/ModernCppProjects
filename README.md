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

**Goal:** Build a modular ray tracing engine organized around dedicated rendering, scene, geometry, material, input, output, and parallel-processing components, with BVH acceleration, OBJ loading, and animated / distributed render paths.

### 🧩 Concepts Practiced

- Object-oriented design using polymorphic `hittable`, `material`, renderer, scene, and camera abstractions
- `std::unique_ptr` ownership and move semantics across renderer setup, scene construction, and world assembly
- Recursive BVH construction for spatial acceleration
- Scene composition through a dedicated `scene_factory`
- Separation of concerns across `Renderer`, `Input`, `Geometry`, `Materials`, `Output`, `Algorithms`, `Data`, `Types`, and `Shared`
- OBJ mesh loading and scene import workflows
- Parallel and distributed rendering structure using `MPI`, parallel camera components, and parallel renderer support
- Support for animated rendering paths through a dedicated animation renderer

### 🎯 Features

- Multiple built-in scenes, including random spheres, checker scenes, Earth / Perlin spheres, quads, Cornell box, smoke, and OBJ-driven scenes
- BVH acceleration for faster ray-object intersection tests
- Support for geometric primitives and scene objects such as spheres, quads, planes, triangles, meshes, and volumetric media
- Material system supporting Lambertian, metal, dielectric, emissive, and texture-driven surfaces
- Texture support including checker textures, Perlin noise, and image-based textures
- Scene setup through `scene_factory` and related input modules
- OBJ / model loading pipeline for external geometry
- Parallel rendering architecture with MPI-based coordination and parallel camera / renderer components
- Output pipeline for writing rendered results to files
- Animated scene support through `renderer_animation`

### 🗂️ Project Design

- `Renderer`: top-level rendering orchestration, including standard and animation renderers
- `Input`: scene factory, scene selection, and OBJ/model loading
- `Geometry`: hittables, primitives, spatial bounds, textures/media-related geometry support
- `Materials`: surface shading and material models
- `Algorithms`: BVH, camera variants, parallel strategies, and rendering-support algorithms
- `Types`: foundational math / rendering types such as `vec3`, `ray`, and `color`
- `Data`: render buffers and shared render data containers
- `Output`: file-writing and image output logic
- `Shared`: project-wide shared utilities and constants

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
