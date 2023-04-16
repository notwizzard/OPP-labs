#include <cmath>
#include "RootSolutionV1.hpp"
#include <mpi.h>

void RootSolutionV1::solve() {
    double error;
    do {
        updateLocalXVector();
        MPI_Allgatherv(&XVector[beginIndex], chunkSize, MPI_DOUBLE, &XVector[0], recvcounts.data(), displs.data(), MPI_DOUBLE, MPI_COMM_WORLD);
        updateLocalAXMatrix();
        double localNormAXSubB = countLocalNormAXSubB();

        double allNormAXSubB;
        MPI_Reduce(&localNormAXSubB, &allNormAXSubB, 1, MPI_DOUBLE, MPI_SUM, localRank, MPI_COMM_WORLD);
        error = std::sqrt(allNormAXSubB) / normB;

        running = error >= EPSILON;
        MPI_Bcast(&running, 1, MPI_CXX_BOOL, localRank, MPI_COMM_WORLD);
    } while (running);

    std::cout << "[ROOT " << localRank << "] solved | check answer status: " << checkAnswer() << std::endl;
//    for (int i = 0; i < vectorSize; i++) {
//        std::cout << XVector[i] << " ";
//    }
}

bool RootSolutionV1::checkAnswer() {
    for (int i = 0; i < vectorSize; i++) {
        if ((int) round(XVector[i]) != 1) {
            return false;
        }
    }
    return true;
}
