#include "AbstractSolution.hpp"

#ifndef LAB1_SOLUTIONV2_HPP
#define LAB1_SOLUTIONV2_HPP


class SolutionV2 : public AbstractSolution {
protected:

    void init() override;
    void updateLocalXVector() override;
    void updateLocalAXMatrix() override;
    double countLocalNormAXSubB() override;
    bool checkAnswer();

public:
    SolutionV2(int localRank, int worldSize, int vectorSize, double epsilon, double tau);
    void solve() override;
};


#endif //LAB1_SOLUTIONV2_HPP
