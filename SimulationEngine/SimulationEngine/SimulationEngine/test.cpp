#define CATCH_CONFIG_RUNNER
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>
#include <mpi.h>

#include "test.hpp"


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);


	int result = Catch::Session().run(argc, argv);


	MPI_Finalize();
}

