#include "SolutionFactory.hpp"

SolutionV1 *SolutionFactory::getSolution(int rank, int worldSize, int vectorSize, double epsilon, double tau) {
    if (rank == 0) {
        return new RootSolutionV1(rank, worldSize, vectorSize, epsilon, tau);
    }
    else {
        return new CommonSolutionV1(rank, worldSize, vectorSize, epsilon, tau);
    }
}
