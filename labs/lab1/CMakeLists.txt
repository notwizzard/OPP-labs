cmake_minimum_required(VERSION 3.5)
project(lab1)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER "/opt/intel/impi/5.0.1.035/intel64/bin/mpicc")
set(CMAKE_CXX_COMPILER "/opt/intel/impi/5.0.1.035/intel64/bin/mpicxx")

set(CMAKE_CXX_FLAGS "-O3")

add_executable(lab1 main.cpp)
add_subdirectory(sources)
target_link_libraries(lab1 PUBLIC sources)