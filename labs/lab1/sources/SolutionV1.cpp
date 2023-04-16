#include <cmath>
#include <iostream>
#include "SolutionV1.hpp"
#include "Utils.hpp"


void SolutionV1::init() {
    AMatrix.resize(chunkSize);
    for (int i = 0; i < chunkSize; i++) {
        XVector.resize(vectorSize, 0);
        BVector.resize(vectorSize, vectorSize + 1);
        AXVector.resize(chunkSize, 0);
        AMatrix[i].resize(vectorSize, 1);
        AMatrix[i][i + beginIndex] = 2;
        normB += BVector[i] * BVector[i];
    }
    normB = std::sqrt(normB);

    for (int i = 0; i < worldSize; i++) {
        recvcounts.push_back(Utils::chunkSize(i, worldSize, vectorSize));
        displs.push_back(Utils::beginIndex(i, worldSize, vectorSize));
    }
}

SolutionV1::SolutionV1(int localRank, int worldSize, int vectorSize) :
    AbstractSolution(localRank, worldSize, vectorSize) {
    init();
}

void SolutionV1::updateLocalXVector() {
    for (int i = 0; i < chunkSize; i++) {
        XVector[i + beginIndex] -= TAU * (AXVector[i] - BVector[i + beginIndex]);
    }
}

void SolutionV1::updateLocalAXMatrix() {
    for (int i = 0; i < chunkSize; i++) {
        AXVector[i] = 0;
        for (int j = 0; j < vectorSize; j++) {
            AXVector[i] += AMatrix[i][j] * XVector[j];
        }
    }
}

double SolutionV1::countLocalNormAXSubB() {
    double normAxSubB = 0;
    for (int i = 0; i < chunkSize; i++) {
        normAxSubB += (AXVector[i] - BVector[i + beginIndex]) * (AXVector[i] - BVector[i + beginIndex]);
    }
    return normAxSubB;
}

void SolutionV1::solve() {

}


