cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_CXX_COMPILER)
    find_program(MPICPX_EXECUTABLE NAMES mpicpx)
    if(MPICPX_EXECUTABLE)
        set(CMAKE_CXX_COMPILER "${MPICPX_EXECUTABLE}" CACHE FILEPATH "MPI C++ compiler" FORCE)
    endif()
endif()

project(Ray_Tracing_38 LANGUAGES CXX)

if(NOT CMAKE_CXX_COMPILER MATCHES "mpicpx")
    message(WARNING "CMAKE_CXX_COMPILER is '${CMAKE_CXX_COMPILER}'. Configure with mpicpx if that is the required MPI wrapper.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Ray_Tracing_38
    RayTracing_21.cpp
    Algorithms/bvh.cpp
    Algorithms/camera.cpp
    Algorithms/camera_drived.cpp
    Algorithms/camera_parallel.cpp
    Algorithms/hittable_list.cpp
    Algorithms/hittable_list_parallel.cpp
    Algorithms/hit_record.cpp
    Algorithms/parallel.cpp
    Algorithms/path.cpp
    Algorithms/rtw_stb_image.cpp
    Algorithms/sphere_list.cpp
    Data/color_array.cpp
    Geometry/aabb.cpp
    Geometry/constant_medium.cpp
    Geometry/hittable.cpp
    Geometry/interval.cpp
    Geometry/mesh.cpp
    Geometry/perlin.cpp
    Geometry/planes.cpp
    Geometry/quad.cpp
    Geometry/sphere.cpp
    Geometry/triangle_mesh.cpp
    Input/input.cpp
    Input/obj_model_reader.cpp
    Input/obj_model_reader_parallel.cpp
    Input/scene_factory.cpp
    Output/write.cpp
    Renderer/Renderer.cpp
    Renderer/Renderer_Animation.cpp
    Types/color.cpp
    Types/vec3.cpp
)

target_include_directories(Ray_Tracing_38 PRIVATE
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

find_package(MPI COMPONENTS CXX QUIET)
if(MPI_CXX_FOUND)
    target_link_libraries(Ray_Tracing_38 PRIVATE MPI::MPI_CXX)
endif()

if(MSVC)
    target_compile_options(Ray_Tracing_38 PRIVATE /W3 /permissive-)
else()
    target_compile_options(Ray_Tracing_38 PRIVATE -Wall -Wextra)
endif()

set_target_properties(Ray_Tracing_38 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
