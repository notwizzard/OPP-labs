#include "SolutionV2.hpp"
#include "Utils.hpp"
#include <cmath>
#include <iostream>
#include <mpi.h>
#include <chrono>


SolutionV2::SolutionV2(int localRank, int worldSize, int vectorSize, double epsilon, double tau) :
        AbstractSolution(localRank, worldSize, vectorSize, epsilon, tau) {
    init();
}

void SolutionV2::init() {
    AMatrix.resize(chunkSize);
    double tempNormB = 0;
    for (int i = 0; i < chunkSize; i++) {
        XVector.resize(chunkSize + 1, 0);
        BVector.resize(chunkSize + 1, vectorSize + 1);
        AXVector.resize(chunkSize, 0);
        AMatrix[i].resize(vectorSize, 1);
        AMatrix[i][i + beginIndex] = 2;
        tempNormB += BVector[i] * BVector[i];
    }

    MPI_Allreduce(&tempNormB, &normB, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    normB = std::sqrt(normB);

    for (int i = 0; i < worldSize; i++) {
        recvcounts.push_back(Utils::chunkSize(i, worldSize, vectorSize));
        displs.push_back(Utils::beginIndex(i, worldSize, vectorSize));
    }
}

void SolutionV2::solve() {
    auto start = std::chrono::high_resolution_clock::now();
    double error;
    do {
        updateLocalXVector();

        updateLocalAXMatrix();

        double localNormAXSubB = countLocalNormAXSubB();
        double allNormAXSubB;
        MPI_Allreduce(&localNormAXSubB, &allNormAXSubB, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        error = std::sqrt(allNormAXSubB) / normB;
        running = error >= EPSILON;
    } while (running);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    if (localRank == 0) {
        std::cout << "[" << localRank << " rank] solved in " << duration.count() << " microseconds | check answer status: " << checkAnswer() << std::endl;
    }
}

bool SolutionV2::checkAnswer() {
    for (int i = 0; i < chunkSize; i++) {
        if ((int) round(XVector[i]) != 1) {
            return false;
        }
    }
    return true;
}

double SolutionV2::countLocalNormAXSubB() {
    double normAxSubB = 0;
    for (int i = 0; i < chunkSize; i++) {
        normAxSubB += (AXVector[i] - BVector[i]) * (AXVector[i] - BVector[i]);
    }
    return normAxSubB;
}

void SolutionV2::updateLocalXVector() {
    for (int i = 0; i < chunkSize; i++) {
        XVector[i] -= TAU * (AXVector[i] - BVector[i]);
    }
}

void SolutionV2::updateLocalAXMatrix() {
    for (int i = 0; i < chunkSize; i++) {
        AXVector[i] = 0;
    }

    for (int shiftNum = 0; shiftNum < worldSize; shiftNum++) {
        for (int rowNum = 0; rowNum < chunkSize; rowNum++) {
            int curBeginIndex = displs[(shiftNum + localRank) % worldSize];
            int curEndIndex = curBeginIndex + recvcounts[(shiftNum + localRank) % worldSize];
            for (int colNum = curBeginIndex; colNum < curEndIndex; colNum++) {
                AXVector[rowNum] += XVector[colNum - curBeginIndex] * AMatrix[rowNum][colNum];
            }
        }

        MPI_Status status;
        MPI_Sendrecv(&XVector[0],
                     recvcounts[(shiftNum + localRank) % worldSize],
                     MPI_DOUBLE,
                     (localRank - 1 + worldSize) % worldSize,
                     0,
                     &XVector[0],
                     recvcounts[(shiftNum + localRank + 1) % worldSize],
                     MPI_DOUBLE,
                     (localRank + 1) % worldSize,
                     0,
                     MPI_COMM_WORLD,
                     &status);
    }
}
