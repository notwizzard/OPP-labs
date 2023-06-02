#include <iostream>
#include <mpi.h>
#include "ThreadService.cpp"

int main(int argc, char* argv[]) {

    int localRank, worldSize;
    int required = MPI_THREAD_MULTIPLE;
    int provided;
    MPI_Init_thread(&argc, &argv, required, &provided);
    if (provided != required) {
        return EXIT_FAILURE;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &localRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    ThreadService* threadService = new ThreadService(localRank, worldSize);
    threadService->execute();
    MPI_Barrier(MPI_COMM_WORLD);
    std::cout << "====================" << std::endl << "ITERATION ENDED" << std::endl << "====================" << std::endl;
    std::flush(std::cout);
    MPI_Finalize();
}
