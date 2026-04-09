#include "MPI_comm_strategy.h"

MPI_comm_strategy::MPI_comm_strategy()
{
	world = MPI_COMM_WORLD;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &size);
}