cmake_minimum_required(VERSION 3.0)

project(dirichlet-problem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

file(GLOB_RECURSE  DIRICHLET_PROBLEM_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

file(GLOB_RECURSE  DIRICHLET_PROBLEM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(dirichlet-problem ${DIRICHLET_PROBLEM_SRC} ${DIRICHLET_PROBLEM_HEADERS})

# MPI
find_package(MPI REQUIRED)
include_directories(SYSTEM "${MPI_CXX_INCLUDE_DIRS}")
target_link_libraries(dirichlet-problem ${MPI_CXX_LIBRARIES})

# OpenMP
if(MSVC)
    target_compile_options(dirichlet-problem PUBLIC "/openmp")
elseif(UNIX)
    target_link_libraries(dirichlet-problem -fopenmp)
else()
    target_compile_options(dirichlet-problem PUBLIC "-fopenmp")
    target_link_libraries(dirichlet-problem -fopenmp)
endif()
