#include "SolutionV1.hpp"

#ifndef COMMON_SOLUTION
#define COMMON_SOLUTION

class CommonSolutionV1 : public SolutionV1 {
public:
    void solve() override;
    CommonSolutionV1(int localRank, int worldSize, int vectorSize, double epsilon, double tau) : SolutionV1(localRank, worldSize, vectorSize, epsilon, tau) {};
};

#endif
