cmake_minimum_required(VERSION 3.20)
project(lab2 C)

set(CMAKE_C_STANDARD 11)

# Find OpenMP package
find_package(OpenMP REQUIRED)

add_executable(lab2 main.c)

# Enable OpenMP
target_link_libraries(lab2 PRIVATE OpenMP::OpenMP_C m)
