# 🚀 Mandelbrot Parallel Renderer with Layout Variants

This project demonstrates the rendering of the Mandelbrot fractal set using various data layout strategies and parallel processing techniques in modern C++. The core focus is on:

* **Exploring memory layout performance** (row-major, column-major, C-style, C++-style, modern heap, `mdspan`)
* **Encapsulating 2D array access via polymorphic interfaces**
* **Applying OpenMP-based parallelism** to accelerate computation
* **Building Mandelbrot animation**

---

## 🎯 Project Goals

* Benchmark and visualize the performance impact of different array storage layouts
* Provide a safe, clean, and extensible foundation for 2D grid-based numerical computation
* Integrate OpenMP for simple and efficient data-parallel Mandelbrot rendering

---

## ✅ Key Concepts Demonstrated

* RAII with `std::unique_ptr` for safe resource management
* `std::make_unique` vs. `new` vs. `malloc` allocation comparisons
* Abstract base class `array` for unified 2D access
* Multiple concrete implementations (namespaced under `Array_NS`):

  * `array_c<allocation_major::X_MAJOR | Y_MAJOR>`
  * `array_cpp<allocation_major::X_MAJOR | Y_MAJOR>`
  * `array_modern<allocation_major::X_MAJOR | Y_MAJOR>` (uses `std::unique_ptr`/`std::vector`)
  * `array_mdspan<allocation_major::X_MAJOR | Y_MAJOR>` (uses `std::mdspan`)
* Centralized `array_allocator` factory to safely manage polymorphic array ownership
* OpenMP (outer-loop and inner-loop variants) to leverage CPU cores
* File output via `std::ofstream`, with options for layout inspection
* Clean separation of concerns:
  * **Algorithm orchestration** (`run_mandelbrot`)  
  * **Parallel tiling/partitioning** (`mandelbrot_{xmesh, ymesh}{_innerloop, _outerloop}`)  
  * **Storage layout policy** (`array_*` and `array_allocator`)  

---

## 🧱 Project Structure

```
Mandelbrot/
├── Algorithm/
│   ├── mandelbrot.h/.cpp                   # Core algorithm + I/O
│   ├── mandelbrot_xmesh.h/.cpp             # X-split parallel base (OpenMP)
│   ├── mandelbrot_xmesh_innerloop.h/.cpp   # Parallelize inner loop (x-mesh)
│   ├── mandelbrot_xmesh_outerloop.h/.cpp   # Parallelize outer loop (x-mesh)
│   ├── mandelbrot_ymesh.h/.cpp             # Y-split parallel base (OpenMP)
│   ├── mandelbrot_ymesh_innerloop.h/.cpp   # Parallelize inner loop (y-mesh)
│   ├── mandelbrot_ymesh_outerloop.h/.cpp   # Parallelize outer loop (y-mesh)
│   ├── run_mandelbrot.h/.cpp               # Orchestrator: timing/animation
│
├── Array/
│   ├── array.h                             # Base abstract interface (Array_NS)
│   ├── array.cpp                           # Base helpers (write_data, etc.)
│   ├── array_c.h                           # C-style allocation (x/y major)
│   ├── array_cpp.h                         # C++ new/delete (x/y major)
│   ├── array_modern.h                      # Modern RAII (vector/unique_ptr)
│   ├── array_mdspan.h                      # std::mdspan-backed storage
│   ├── array_allocator.h                   # Polymorphic factory
│
├── Numerical/
│   └── complex.h                           # Lightweight complex (Numerical_NS)
│
├── Plotting/
│   └── Mandelbrot_plot_frames.py           # Frame -> image helper
│
├── definitions.h                           # Common types/config (enums, thread_config)
└── README.md
```

---

## 🛠 Build Instructions

> **Requirements**
> * A compiler with **OpenMP** support (`-fopenmp` GCC/Clang, `/openmp` MSVC).
> * For `std::mdspan`, a **C++23**-capable standard library (or disable MDSPAN in your build if unavailable).

### GCC / Clang (Linux/macOS)

```bash
# From the Mandelbrot/ folder
g++ -std=c++23 -O3 -fopenmp   Algorithm/*.cpp Array/array.cpp   -IAlgorithm -IArray -INumerical   -o mandelbrot

./mandelbrot
```

If your toolchain doesn’t support `std::mdspan` yet, either:
- switch `allocation_mode` away from `MDSPAN`, or
- compile with a `-DUSE_MDSPAN=0` flag you check in code (optional), or
- use a C++23-ready compiler/libc++/libstdc++.

### MSVC (Windows, Developer Prompt)

```bat
cl /std:c++23 /O2 /openmp /EHsc ^
  Algorithm\*.cpp Array\array.cpp ^
  /IAlgorithm /IArray /INumerical ^
  /Fe:mandelbrot.exe
```

---

## 📸 Output

Each implementation renders the Mandelbrot set to a `.dat` file (CSV-like).  
Use the provided Python helper to convert frames to images:

```bash
python3 Plotting/Mandelbrot_plot_frames.py <data-prefix> <frame_init> <stride> <frame_last> <x_dim> <y_dim>
# Example:
# python3 Plotting/Mandelbrot_plot_frames.py frame 0 1 100 1920 1080
```

---

## 📌 Future Plans

* Complete redesign of `mandelbrot_???` classes for layout-agnostic rendering
* Adding MPI and threads support to compare their performance with OpenMP
* Integrate PPM/PGM export support
* Visual comparison of layout access performance
* Interactive CLI for resolution, bounds, and layout switching
* Unit tests for partitioning logic and array indexers (to guarantee no gaps/overlaps)

---

## ✍️ Author

**Mahdi Tavakol**  
Postdoctoral Researcher | Computational Scientist  
📧 [mahditavakol90@gmail.com](mailto:mahditavakol90@gmail.com)

> If you find this project useful, ⭐ it on GitHub and feel free to reach out!
