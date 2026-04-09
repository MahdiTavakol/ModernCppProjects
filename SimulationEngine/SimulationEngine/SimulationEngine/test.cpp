#define CATCH_CONFIG_RUNNER
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>
#include <mpi.h>
#include <unordered_set>

#include "test.hpp"

enum {SERIAL, MPI};


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int mode = MPI;

	for (int i = 0; i < argc; i++)
	{
		if (argv[i] == "--mpi")
			mode = MPI;
		else if (argv[i] == "--serial")
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
		session.run();

	//int result = Catch::Session().run(argc, argv);


	MPI_Finalize();
}

