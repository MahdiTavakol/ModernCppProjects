# 🚀 Mandelbrot Parallel Renderer with Layout Variants

This project demonstrates the rendering of the Mandelbrot fractal set using various data layout strategies and parallel processing techniques in modern C++. The core focus is on:

* **Exploring memory layout performance** (row-major, column-major, C-style, C++-style, modern heap)
* **Encapsulating 2D array access via polymorphic interfaces**
* **Applying OpenMP-based parallelism** to accelerate computation

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
* Multiple concrete implementations:

  * `array_c_x_major`
  * `array_c_y_major`
  * `array_cpp_x_major`
  * `array_cpp_y_major`
  * `array_c_modern_x_major`
  * `array_c_modern_y_major`
* Centralized `array_allocator` factory to safely manage polymorphic array ownership
* OpenMP (`#pragma omp parallel for`) to leverage CPU cores
* File output via `std::ofstream`, with options for layout inspection

---

## 🧱 Project Structure

```
Mandelbrot/
├── array.h                        # Base abstract interface
├── array_c_x_major.h/.cpp         # C-style col-major layout
├── array_c_y_major.h/.cpp         # C-style row-major layout
├── array_cpp_x_major.h/.cpp       # C++-style heap col-major layout
├── array_cpp_y_major.h/.cpp       # C++-style heap row-major layout
├── array_c_modern_x_major.h/.cpp  # Modern C++ heap col-major
├── array_c_modern_y_major.h/.cpp  # Modern C++ heap row-major
├── array_allocator.h              # Polymorphic allocator factory
├── main.cpp                       # Entry point with OpenMP renderer
├── (coming soon) mandelbrot_*.cpp/.h
└── README.md
```

---

## 🛠 Build Instructions

```bash
g++ -std=c++20 -fopenmp -O3 main.cpp array_*.cpp -o mandelbrot
./mandelbrot
```

> Make sure you have OpenMP support enabled (e.g., `libgomp` on Linux).

---

## 📸 Output

Each implementation renders a segment of the Mandelbrot set into a file (e.g., CSV or raw PPM), depending on layout type and configuration. Output timing and access pattern can be compared.

---

## 📌 Future Plans

* Complete redesign of `mandelbrot_???` classes for layout-agnostic rendering
* Integrate PPM/PGM export support
* Visual comparison of layout access performance
* Interactive CLI for resolution, bounds, and layout switching

---

## ✍️ Author

**Mahdi Tavakol**
Postdoctoral Researcher | Computational Scientist
📧 [mahditavakol90@gmail.com](mailto:mahditavakol90@gmail.com)

> If you find this project useful, ⭐ it on GitHub and feel free to reach out!
