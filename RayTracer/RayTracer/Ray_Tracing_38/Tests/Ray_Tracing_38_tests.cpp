#define CATCH_CONFIG_RUNNER
#include "catch_amalgamated.hpp"


#include "test_shared.h"
#include <sstream>

#include "../Algorithms/mpiParallel.h"

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);


	int result = Catch::Session().run(argc, argv);


	MPI_Finalize();
}








