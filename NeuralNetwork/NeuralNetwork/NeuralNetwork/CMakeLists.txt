cmake_minimum_required(VERSION 3.16)
project(NeuralNetworkMPI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" "")
endif()

# Optional warnings
if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---------- Portable deps ----------
find_package(OpenMP)
option(ENABLE_MPI "Build MPI components" ON)
if(ENABLE_MPI)
  find_package(MPI)
endif()

# ---------- Targets with UNIQUE names ----------
set(CORE_TARGET   nn_core_lib)     # was nn_core
set(APP_TARGET    nn_app_exe)      # don't reuse ${PROJECT_NAME}
set(TEST_TARGET   nn_tests_exe)    # avoid generic names like "tests" or "nn_tests"

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

add_executable(${APP_TARGET} main.cpp)
target_link_libraries(${APP_TARGET} PRIVATE ${CORE_TARGET})

# ---------- Catch2 tests ----------
option(BUILD_TESTS "Build Catch2 tests" OFF)
if(BUILD_TESTS)
  include(FetchContent)
  # Use lowercase handle; FetchContent exposes catch2_SOURCE_DIR / _BINARY_DIR
  FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
  )
  FetchContent_MakeAvailable(catch2)

  enable_testing()

  add_executable(${TEST_TARGET}
    tests/test_basic.cpp
    # add more test files here
  )
  target_link_libraries(${TEST_TARGET} PRIVATE
    ${CORE_TARGET}
    Catch2::Catch2WithMain
  )

  # Prefer Catch’s test discovery if its module exists;
  # otherwise fall back to a simple CTest entry.
  if(EXISTS "${catch2_SOURCE_DIR}/extras/Catch.cmake")
    list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
    include(Catch)                              # now found
    # MUST be in the SAME directory where ${TEST_TARGET} was created
    catch_discover_tests(${TEST_TARGET})
  else()
    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
  endif()
endif()
