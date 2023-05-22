//
// Created by mimor on 20-Mar-23.
//

#include "AbstractSolution.hpp"
#include "Utils.hpp"

AbstractSolution::AbstractSolution(int localRank, int worldSize, int vectorSize, double epsilon, double tau)
    : localRank(localRank), worldSize(worldSize), vectorSize(vectorSize), EPSILON(epsilon), TAU(tau) {
    beginIndex = Utils::beginIndex(localRank, worldSize, vectorSize);
    endIndex = Utils::endIndex(localRank, worldSize, vectorSize);
    chunkSize = Utils::chunkSize(localRank, worldSize, vectorSize);
}

void AbstractSolution::updateLocalXVector() {

}

void AbstractSolution::init() {

}

double AbstractSolution::countLocalNormAXSubB() {
    return 0;
}

void AbstractSolution::updateLocalAXMatrix() {

}

void AbstractSolution::solve() {

}
