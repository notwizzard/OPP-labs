#include <vector>
#include "AbstractSolution.hpp"

#ifndef SOLUTION
#define SOLUTION

class SolutionV1 : public AbstractSolution {
protected:

    void init() override;
    void updateLocalXVector() override;
    void updateLocalAXMatrix() override;
    double countLocalNormAXSubB() override;

public:
    SolutionV1(int localRank, int worldSize, int vectorSize);
    virtual void solve() override;
};

#endif
