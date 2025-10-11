cmake_minimum_required(VERSION 3.16)
project(NeuralNetworkMPI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Multi-config generators (like MSVC) ignore CMAKE_BUILD_TYPE; that's fine.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" "")
endif()

# Build switches
option(BUILD_APP   "Build main application" ON)
option(BUILD_TESTS "Build unit tests"       OFF)   # OFF by default
option(ENABLE_MPI  "Build MPI components"   ON)

# Warnings
if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Deps
find_package(OpenMP)
if(ENABLE_MPI)
  find_package(MPI)
endif()

# ---------- Targets with UNIQUE names ----------
set(CORE_TARGET   nn_core_lib)
set(APP_TARGET    nn_app_exe)
set(TEST_TARGET   nn_tests_exe)

set(COMMON_SOURCES
  Activations.cpp
  Dropout.cpp
  InputArgs.cpp
  InputFile.cpp
  LastLayerBatchEfficientSoftMax.cpp
  LayerBatchEfficient.cpp
  Loss.cpp
  Scaler.cpp
  NeuralNetwork.cpp
  NeuralNetworkOpenMP.cpp
  Neuron.cpp
  Optimizers.cpp
)

set(MPI_SOURCES
  InputMPIFile.cpp
  NeuralNetworkMPI.cpp
)

add_library(${CORE_TARGET} ${COMMON_SOURCES})
target_include_directories(${CORE_TARGET} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../eigen-master
)
if(OpenMP_CXX_FOUND)
  target_link_libraries(${CORE_TARGET} PUBLIC OpenMP::OpenMP_CXX)
endif()
if(ENABLE_MPI AND MPI_CXX_FOUND)
  target_sources(${CORE_TARGET} PRIVATE ${MPI_SOURCES})
  target_link_libraries(${CORE_TARGET} PUBLIC MPI::MPI_CXX)
  target_compile_definitions(${CORE_TARGET} PUBLIC NN_WITH_MPI=1)
else()
  message(STATUS "MPI not found or disabled → building without MPI sources")
endif()

# -------- App (toggle with BUILD_APP) -------
if(BUILD_APP)
  add_executable(${APP_TARGET} main.cpp)
  target_link_libraries(${APP_TARGET} PRIVATE ${CORE_TARGET})
endif()

# -------- Tests (toggle with BUILD_TESTS) ---
if(BUILD_TESTS)
  enable_testing()

  # Minimal Catch2 v2 header-only layout (you said you include catch.hpp)
  # Assume tests live under ./tests
  file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
       "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

  add_executable(${TEST_TARGET} ${TEST_SOURCES})
  target_link_libraries(${TEST_TARGET} PRIVATE ${CORE_TARGET})

  # If catch.hpp is in tests/:
  target_include_directories(${TEST_TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests)

  # Register a single ctest that runs all Catch2 tests
  add_test(NAME unit COMMAND ${TEST_TARGET})
endif()
