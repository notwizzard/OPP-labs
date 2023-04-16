#include "SolutionV1.hpp"
#include "RootSolutionV1.hpp"
#include "CommonSolutionV1.hpp"

#ifndef SOLUTION_FACTORY
#define SOLUTION_FACTORY

class SolutionFactory {
public:
    static SolutionV1* getSolution(int rank, int worldSize, int vectorSize);
};

#endif

