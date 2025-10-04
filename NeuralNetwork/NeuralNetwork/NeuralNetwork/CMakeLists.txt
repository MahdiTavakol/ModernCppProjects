cmake_minimum_required(VERSION 3.16)
project(NeuralNetworkMPI LANGUAGES CXX)

# ---- Language / configs ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release if not set (no FORCE so Debug is selectable)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" "")
endif()

# Helpful warnings per-compiler (optional)
if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---- Dependencies (portable) ----
find_package(OpenMP)
option(ENABLE_MPI "Build MPI components" ON)
if(ENABLE_MPI)
  find_package(MPI)
endif()

# ---- Sources ----
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

# ---- Core library (no main.cpp) ----
add_library(nn_core ${COMMON_SOURCES})
target_include_directories(nn_core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../eigen-master
)

if(OpenMP_CXX_FOUND)
  target_link_libraries(nn_core PUBLIC OpenMP::OpenMP_CXX)
endif()

if(ENABLE_MPI AND MPI_CXX_FOUND)
  target_sources(nn_core PRIVATE ${MPI_SOURCES})
  target_link_libraries(nn_core PUBLIC MPI::MPI_CXX)
  target_compile_definitions(nn_core PUBLIC NN_WITH_MPI=1)
else()
  message(STATUS "MPI not found or disabled → building without MPI sources")
endif()

# ---- App exe ----
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nn_core)

# ---- (Optional) Catch2 tests ----
option(BUILD_TESTS "Build Catch2 tests" ON)
if(BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
  )
  FetchContent_MakeAvailable(Catch2)

  enable_testing()
  add_executable(nn_tests tests/test_basic.cpp)
  target_link_libraries(nn_tests PRIVATE nn_core Catch2::Catch2WithMain)
  include(Catch)
  catch_discover_tests(nn_tests)
endif()
