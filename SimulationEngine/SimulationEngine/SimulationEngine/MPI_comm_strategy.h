#pragma once
#include "Comm_strategy.h"
#include <mpi.h>

class MPI_comm_strategy : public Comm_strategy
{
public:
	MPI_comm_strategy();

private:
	MPI_Comm world;
};
