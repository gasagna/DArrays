#define CATCH_CONFIG_RUNNER
#include <iostream>

#include <catch.hpp>
#include "DArrays.hpp"


int main(int argc, char* argv[]) {
    DArrays::MPI::Initialize();

    // ensure we are running tests on 2 processors
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( size != 2 ) {
        if ( rank == 0 )
            std::cout << "test suite must be run with 2 processors! Aborting!\n";
    } else {
        int result = Catch::Session().run(argc, argv);
    }

    DArrays::MPI::Finalize();
    return 0;
}