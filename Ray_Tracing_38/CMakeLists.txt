cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_CXX_COMPILER)
    find_program(MPICPX_EXECUTABLE NAMES mpicpx)
    if(MPICPX_EXECUTABLE)
        set(CMAKE_CXX_COMPILER "${MPICPX_EXECUTABLE}" CACHE FILEPATH "MPI C++ compiler" FORCE)
    endif()
endif()

project(Ray_Tracing_38 LANGUAGES C CXX)

option(BUILD_TESTS "Build Catch2 tests instead of the main executable" OFF)

if(NOT CMAKE_CXX_COMPILER MATCHES "mpicpx")
    message(WARNING "CMAKE_CXX_COMPILER is '${CMAKE_CXX_COMPILER}'. Configure with mpicpx if that is the required MPI wrapper.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE RAYTRACER_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

list(FILTER RAYTRACER_SOURCES EXCLUDE REGEX "/(Tests|x64|Intel® VTune™ Profiler Results)/")

if(BUILD_TESTS)
    file(GLOB_RECURSE TEST_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.c
    )

    add_executable(Ray_Tracing_38_tests ${TEST_SOURCES})
    set(TARGET_NAME Ray_Tracing_38_tests)
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms
        ${CMAKE_CURRENT_SOURCE_DIR}/Data
        ${CMAKE_CURRENT_SOURCE_DIR}/Geometry
        ${CMAKE_CURRENT_SOURCE_DIR}/Input
        ${CMAKE_CURRENT_SOURCE_DIR}/Materials
        ${CMAKE_CURRENT_SOURCE_DIR}/Output
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
        ${CMAKE_CURRENT_SOURCE_DIR}/Shared
        ${CMAKE_CURRENT_SOURCE_DIR}/Types
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        ${CMAKE_CURRENT_SOURCE_DIR}/Tests
    )
else()
    add_executable(Ray_Tracing_38 ${RAYTRACER_SOURCES})
    set(TARGET_NAME Ray_Tracing_38)
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Algorithms
        ${CMAKE_CURRENT_SOURCE_DIR}/Data
        ${CMAKE_CURRENT_SOURCE_DIR}/Geometry
        ${CMAKE_CURRENT_SOURCE_DIR}/Input
        ${CMAKE_CURRENT_SOURCE_DIR}/Materials
        ${CMAKE_CURRENT_SOURCE_DIR}/Output
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
        ${CMAKE_CURRENT_SOURCE_DIR}/Shared
        ${CMAKE_CURRENT_SOURCE_DIR}/Types
        ${CMAKE_CURRENT_SOURCE_DIR}/external
    )
endif()

find_package(MPI COMPONENTS CXX QUIET)
if(MPI_CXX_FOUND)
    target_link_libraries(${TARGET_NAME} PRIVATE MPI::MPI_CXX)
endif()

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W3 /permissive-)
else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra)
endif()

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
