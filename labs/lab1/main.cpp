#include <iostream>
#include <mpi.h>
#include "SolutionV1.hpp"
#include "SolutionV2.hpp"
#include "SolutionFactory.hpp"

//#define epsilon 0.0000000001
//#define tau 0.00001
//#define N 10000


int main(int argc, char **argv) {
    double epsilon = std::strtod(argv[2], NULL);
    double tau = std::strtod(argv[3], NULL);
    int N = std::atoi(argv[1]);
    
    int localRank, worldSize;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &localRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    std::cout << worldSize << std::endl;

//    SolutionV1* solutionV1 = SolutionFactory::getSolution(localRank, worldSize, N, epsilon, tau);
//    solutionV1->solve();

    SolutionV2* solutionV2 = new SolutionV2(localRank, worldSize, N, epsilon, tau);
    solutionV2->solve();

    MPI_Finalize();
}
