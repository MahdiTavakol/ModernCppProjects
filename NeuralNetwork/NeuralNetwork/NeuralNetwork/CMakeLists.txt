cmake_minimum_required(VERSION 3.16)
project(NeuralNetworkMPI LANGUAGES CXX)

# Use Intel MPI compiler
set(CMAKE_CXX_COMPILER mpiicpx)

# C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release if not set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Debug flags (only active if CMAKE_BUILD_TYPE=Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -fno-omit-frame-pointer")

# Manually enable Intel OpenMP support for mpiicpx
set(OpenMP_CXX_FLAGS "-qopenmp")
set(OpenMP_CXX_LIB_NAMES "iomp5")
set(OpenMP_iomp5_LIBRARY "${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES}/libiomp5.so")

add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
set_property(TARGET OpenMP::OpenMP_CXX PROPERTY INTERFACE_COMPILE_OPTIONS "${OpenMP_CXX_FLAGS}")
set_property(TARGET OpenMP::OpenMP_CXX PROPERTY INTERFACE_LINK_LIBRARIES iomp5)

# Source files
set(SOURCES
    Activations.cpp
    Dropout.cpp
    Input.cpp
    InputMPI.cpp
    LastLayerBatchEfficient.cpp
    LayerBatchEfficient.cpp
    Loss.cpp
    Scaler.cpp
    NeuralNetwork.cpp
    NeuralNetworkMPI.cpp
    NeuralNetworkOpenMP.cpp
    Neuron.cpp
    Optimizers.cpp
    main.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include paths
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../eigen-master
)

# Link OpenMP
target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)

