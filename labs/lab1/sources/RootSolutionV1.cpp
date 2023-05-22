#include <cmath>
#include "RootSolutionV1.hpp"
#include <mpi.h>
#include <iostream>
#include <chrono>

void RootSolutionV1::solve() {
    auto start = std::chrono::high_resolution_clock::now();
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

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "[ROOT " << localRank << "] solved in " << duration.count() << " microseconds | check answer status: " << checkAnswer() << std::endl;

}

bool RootSolutionV1::checkAnswer() {
    for (int i = 0; i < vectorSize; i++) {
        if ((int) round(XVector[i]) != 1) {
            return false;
        }
    }
    return true;
}
