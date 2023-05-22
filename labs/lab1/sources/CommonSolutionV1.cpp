#include "CommonSolutionV1.hpp"
#include <mpi.h>
#include <iostream>
#include <chrono>


void CommonSolutionV1::solve() { 
   
    do {
        updateLocalXVector();
        MPI_Allgatherv(&XVector[beginIndex], chunkSize, MPI_DOUBLE, &XVector[0], recvcounts.data(), displs.data(), MPI_DOUBLE, MPI_COMM_WORLD);
        updateLocalAXMatrix();
        double localNormAXSubB = countLocalNormAXSubB();

        double allNormAXSubB;
        MPI_Reduce(&localNormAXSubB, &allNormAXSubB, 1, MPI_DOUBLE, MPI_SUM, rootRank, MPI_COMM_WORLD);
        MPI_Bcast(&running, 1, MPI_CXX_BOOL, rootRank, MPI_COMM_WORLD);
    } while (running);    
}
