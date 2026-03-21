# 🚀 BVH Ray Tracer with Scene Factory and Parallel Extensions

This project implements a modular ray tracing engine in modern C++, supporting a wide range of geometric primitives, materials, textures, and imported meshes. The core focus is on:

* Building a scalable ray tracing architecture
* Applying BVH (Bounding Volume Hierarchy) for acceleration
* Designing clean ownership models using std::unique_ptr
* Supporting complex scenes via factories and OBJ mesh loading
* Exploring parallel and distributed rendering approaches

---

## 🎯 Project Goals

* Design a clean and extensible ray tracing architecture beyond toy examples
* Implement BVH acceleration to handle large triangle meshes efficiently
* Separate scene construction, rendering, and output into independent modules
* Enable flexible scene configuration via factory-based scenarios
* Explore parallel rendering strategies (OpenMP / MPI-ready design)

---

## ✅ Key Concepts Demonstrated

* RAII and ownership modeling using std::unique_ptr
* Move semantics and ownership transfer during scene → BVH construction
* Polymorphic design:
  * hittable interface for geometry abstraction
  * material and texture hierarchies
* Recursive BVH construction and traversal
* Scene factory pattern for configurable environments
* Separation of concerns:
  * Scene setup (scene_factory)
  * Acceleration structure (bvh_node)
  * Rendering pipeline (renderer, camera)
  * Output handling (write_to_file)
* Mesh loading pipeline (OBJ + MTL)
* Volumetric rendering support
* Parallel-ready design (MPI-style structure)

---

## 🧱 Project Structure

RayTracing/
├── Geometry/
├── Acceleration/
├── Materials/
├── Textures/
├── Scene/
├── Rendering/
├── IO/
├── Parallel/
├── Core/
├── main.cpp
└── README.md

---

## 🛠 Build Instructions

### GCC / Clang

g++ -std=c++20 -O3 *.cpp -o raytracer
./raytracer

---

## 📸 Output

* Physically-based shading
* Global illumination (Monte Carlo)
* Volumetric effects
* OBJ mesh rendering

---

## 🎬 Available Scenes

* Random spheres
* Cornell box
* Light scenes
* Earth textures
* OBJ models
* Animated scenes

---

## ⚡ Performance Highlights

* BVH reduces intersection cost significantly
* Supports large triangle meshes
* Parallel-ready architecture

---

## 📌 Future Plans

* SAH BVH optimization
* SIMD acceleration
* GPU support
* Interactive rendering
* Unit testing

---

## 🙏 Acknowledgements

This project is inspired by and based on *Ray Tracing in One Weekend* by Peter Shirley et al.  
https://raytracing.github.io/books/RayTracingInOneWeekend.html

The original codebase has been  extended with scene factories, mesh loading, obj files support, MPI parallelization and a more modular architecture.

## ✍️ Author

Mahdi Tavakol  
Postdoctoral Researcher | Computational Scientist  
mahdi.tavakol@eng.ox.ac.uk
