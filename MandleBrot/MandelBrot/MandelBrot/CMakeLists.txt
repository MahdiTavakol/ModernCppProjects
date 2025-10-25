cmake_minimum_required(VERSION 3.20)

# -----------------------------
# Project
# -----------------------------
project(Mandelbrot LANGUAGES CXX VERSION 0.1.0)

# Options
option(USE_MDSPAN "Enable std::mdspan-based arrays (needs C++23-ready standard library)" ON)
option(USE_FAST_MATH "Enable fast-math optimizations when supported" OFF)

# C++ Standard
if (USE_MDSPAN)
  set(CMAKE_CXX_STANDARD 23)
else()
  set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Detect compiler families
# -----------------------------
# CMAKE_<LANG>_COMPILER_ID values:
#   "Intel" (classic ICC/ICPC), "IntelLLVM" (oneAPI icx/icpx), "GNU", "Clang", "MSVC"
message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

# Baseline warnings
if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Optimize by compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "Intel" OR CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
  # oneAPI (icx/icpx) or classic icc/icpc
  if (MSVC)
    # Windows icx in MSVC mode
    target_compile_options(# will be attached later via generator expression
    )
  else()
    add_compile_options(-O3)
    # Prefer -march=native with IntelLLVM (LLVM backend). Classic ICC uses -xHost.
    if (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
      add_compile_options(-march=native)
    else()
      add_compile_options(-xHost)
    endif()
    if (USE_FAST_MATH)
      add_compile_options(-ffast-math)           # supported by IntelLLVM (LLVM)
      # Classic ICC would prefer: -fp-model fast=2
    endif()
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-O3 -march=native)
  if (USE_FAST_MATH)
    add_compile_options(-ffast-math)
  endif()
elseif (MSVC)
  add_compile_options(/O2)
endif()

# -----------------------------
# OpenMP
# -----------------------------
find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
  message(STATUS "OpenMP found: ${OpenMP_CXX_VERSION}")
endif()

# -----------------------------
# Sources
# -----------------------------
set(ALGO_SOURCES
  Algorithm/mandelbrot.cpp
  Algorithm/mandelbrot_xmesh.cpp
  Algorithm/mandelbrot_xmesh_innerloop.cpp
  Algorithm/mandelbrot_xmesh_outerloop.cpp
  Algorithm/mandelbrot_ymesh.cpp
  Algorithm/mandelbrot_ymesh_innerloop.cpp
  Algorithm/mandelbrot_ymesh_outerloop.cpp
)

set(ARRAY_SOURCES
  Array/array.cpp
)

set(RUNNER_SOURCES
  Runner/run_mandelbrot_animation.cpp
  Runner/run_mandelbrot_timing.cpp
)

set(MAIN_SRC
  main.cpp
)

add_executable(mandelbrot
  ${MAIN_SRC}
  ${ALGO_SOURCES}
  ${ARRAY_SOURCES}
  ${RUNNER_SOURCES}
)

# -----------------------------
# Includes
# -----------------------------
target_include_directories(mandelbrot PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}          # for definitions.h at root
  ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm
  ${CMAKE_CURRENT_SOURCE_DIR}/Array
  ${CMAKE_CURRENT_SOURCE_DIR}/Runner
  ${CMAKE_CURRENT_SOURCE_DIR}/Numerical
)

# -----------------------------
# Compile-time feature toggles
# -----------------------------
target_compile_definitions(mandelbrot PUBLIC
  $<$<BOOL:${USE_MDSPAN}>:USE_MDSPAN=1>
  $<$<NOT:$<BOOL:${USE_MDSPAN}>>:USE_MDSPAN=0>
)

# -----------------------------
# Link
# -----------------------------
target_link_libraries(mandelbrot PRIVATE OpenMP::OpenMP_CXX)

# Intel (Windows MSVC-mode) needs OpenMP switch on the target sometimes:
if (MSVC AND (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM"))
  target_compile_options(mandelbrot PRIVATE /Qopenmp)
endif()

# -----------------------------
# IDE source groups (optional)
# -----------------------------
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm PREFIX "Algorithm" FILES ${ALGO_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Array     PREFIX "Array"     FILES ${ARRAY_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Runner    PREFIX "Runner"    FILES ${RUNNER_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}           PREFIX "Root"      FILES ${MAIN_SRC})

