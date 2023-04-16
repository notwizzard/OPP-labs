cmake_minimum_required(VERSION 3.20)
project(lab1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MPI_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MPI_LINK_FLAGS}")

find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
add_executable(lab1 main.cpp)
add_subdirectory(sources)
target_link_libraries(lab1 PUBLIC ${MPI_C_LIBRARIES} sources)

#add_custom_target(run_mpi
#    COMMAND mpiexec ${MPIEXEC_NUMPROC_FLAG} 4 ${MPIEXEC_PREFLAGS} ${CMAKE_BINARY_DIR}/lab1 ${MPIEXEC_POSTFLAGS}
#    DEPENDS lab1
#)