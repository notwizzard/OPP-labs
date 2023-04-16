#include "SolutionV1.hpp"

#ifndef ROOT_SOLUTION
#define ROOT_SOLUTION

class RootSolutionV1 : public SolutionV1 {
private:
    bool checkAnswer();
public:
    void solve() override;
    RootSolutionV1(int localRank, int worldSize, int vectorSize) : SolutionV1(localRank, worldSize, vectorSize) {};
};

#endif
