cmake_minimum_required(VERSION 3.20)
project(Xenon LANGUAGES CXX CUDA)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

set(XENON_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(POLYSCOPE_DIR ${CMAKE_SOURCE_DIR}/ext/polyscope)

add_subdirectory(${POLYSCOPE_DIR})

file(GLOB_RECURSE CPP_SOURCES
    ${XENON_SRC_DIR}/*.cpp
    ${XENON_SRC_DIR}/*.h
)

set(CUDA_SOURCES
    ${XENON_SRC_DIR}/CUDA/CUDACommon.cuh
    ${XENON_SRC_DIR}/CUDA/HashMap.cuh
    ${XENON_SRC_DIR}/CUDA/HashMap.cu
    ${XENON_SRC_DIR}/CUDA/Geometry.cuh
    ${XENON_SRC_DIR}/CUDA/Geometry.cu

    ${XENON_SRC_DIR}/CUDA/main.cuh
    ${XENON_SRC_DIR}/CUDA/main.cu
)

add_library(cuda_kernels STATIC ${CUDA_SOURCES})

target_include_directories(cuda_kernels PRIVATE
    ${POLYSCOPE_DIR}/deps/glad/include
)

set_target_properties(cuda_kernels PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    POSITION_INDEPENDENT_CODE ON
)

target_compile_options(cuda_kernels PRIVATE
    $<$<COMPILE_LANGUAGE:CUDA>:--use_fast_math -lineinfo>
)

add_executable(Xenon ${CPP_SOURCES})

target_include_directories(Xenon PRIVATE
    ${XENON_SRC_DIR}
    ${POLYSCOPE_DIR}/include
)

target_link_libraries(Xenon PRIVATE
    polyscope
    cuda
    cudart
    cuda_kernels
)

if (MSVC)
    target_compile_options(Xenon PRIVATE /bigobj)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Xenon)
endif()
