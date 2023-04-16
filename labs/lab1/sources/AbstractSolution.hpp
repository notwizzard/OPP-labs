#include <vector>

#ifndef LAB1_SOLUTIONINTERFACE_HPP
#define LAB1_SOLUTIONINTERFACE_HPP


class AbstractSolution {
protected:
    static constexpr double EPSILON = 0.0000000001;
    static constexpr double TAU = 0.00001;

    int rootRank = 0;
    int localRank;
    int worldSize;
    int vectorSize;
    double normB = 0;
    bool running = true;

    int beginIndex;
    int endIndex;
    int chunkSize;

    std::vector<double> XVector;
    std::vector<double> BVector;
    std::vector<double> AXVector;
    std::vector<std::vector<double>> AMatrix;

    std::vector<int> recvcounts;
    std::vector<int> displs;

    virtual void init();
    virtual void updateLocalXVector();
    virtual void updateLocalAXMatrix();
    virtual double countLocalNormAXSubB();

public:
    AbstractSolution(int localRank, int worldSize, int vectorSize);
    virtual void solve();
};


#endif //LAB1_SOLUTIONINTERFACE_HPP
