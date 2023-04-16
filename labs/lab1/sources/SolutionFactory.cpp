#include "SolutionFactory.hpp"

SolutionV1 *SolutionFactory::getSolution(int rank, int worldSize, int vectorSize) {
    if (rank == 0) {
        return new RootSolutionV1(rank, worldSize, vectorSize);
    }
    else {
        return new CommonSolutionV1(rank, worldSize, vectorSize);
    }
}
