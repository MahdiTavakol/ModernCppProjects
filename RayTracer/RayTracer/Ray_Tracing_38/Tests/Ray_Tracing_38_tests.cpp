#define CATCH_CONFIG_RUNNER
#include "catch_amalgamated.hpp"


#include "test_shared.h"
#include <sstream>
#include <unordered_set>

#include "../Algorithms/mpiParallel.h"

enum { SERIAL, MPI };


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int mode = MPI;

	for (int i = 0; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "--mpi")
			mode = MPI;
		else if (arg == "--serial")
			mode = SERIAL;
	}

	Catch::Session session;

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	std::unordered_set<int> includedRanks;


	switch (mode)
	{
	case MPI:
		for (int i = 0; i < size; i++)
			includedRanks.insert(i);
		session.configData().testsOrTags = { "[mpi]" };
		break;
	case SERIAL:
		includedRanks.insert(0);
		session.configData().testsOrTags = { "~[mpi]" };
	}

	if (includedRanks.find(rank) != includedRanks.end())
		session.run(argc, argv);




	std::stringstream oss;
	auto* oldCout = std::cout.rdbuf(oss.rdbuf());
	auto* oldCerr = std::cerr.rdbuf(oss.rdbuf());

	std::cout.rdbuf(oldCout);
	std::cerr.rdbuf(oldCerr);

	for (int i = 0; i < size; i++)
	{
		MPI_Barrier(MPI_COMM_WORLD);
		if (i == rank)
		{
			std::cout << "\n=====================================\n";
			std::cout << "Rank " << rank << " output:\n";
			std::cout << "=====================================\n";
			std::cout << oss.str() << std::endl;
		}
	}


	MPI_Finalize();
}








