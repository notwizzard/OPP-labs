cmake_minimum_required(VERSION 3.5)
project(lab5)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_CXX_COMPILER "mpicxx")

# set(CMAKE_CXX_FLAGS "-O3")

add_executable(lab5 main.cpp)
add_subdirectory(sources)
target_link_libraries(lab5 PUBLIC sources)