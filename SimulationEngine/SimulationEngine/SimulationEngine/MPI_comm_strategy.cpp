#include "MPI_comm_strategy.h"

MPI_comm_strategy::MPI_comm_strategy()
{
	world = MPI_COMM_WORLD;
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &size);
}

MPI_comm_strategy::MPI_comm_strategy(MPI_Comm world_, const int& rank_):
	Comm_strategy{rank_},
	world{world_}
{
}

void MPI_comm_strategy::update(double* buff_, const int size_, const int tag_, const int src_, const int dst_) 
{
	MPI_Request request;
	MPI_Status status;
	if (rank == src_)
	{
		MPI_Isend(buff_, size_, MPI_DOUBLE, dst_, tag_, world, &request);
	}
	else if (rank == dst_)
	{
		MPI_Recv(buff_, size_, MPI_DOUBLE, src_, tag_, world, MPI_STATUSES_IGNORE);
	}
}

void MPI_comm_strategy::send(double* buff_, const int size_, const int dst_, const int tag_)
{
	if (num_request == maxRequest)
	{
		// need to thing about it
		num_request = 0;
	}
	MPI_Isend(buff_, size_, MPI_DOUBLE, dst_, tag_, world, &requests[num_request++]);
}

void MPI_comm_strategy::recv(double* buffer_, const int size_, const int src_, const int tag_)
{
	MPI_Recv(buffer_, size_, MPI_DOUBLE, src_, tag_, world, MPI_STATUS_IGNORE);
}

void MPI_comm_strategy::waitAll()
{
	MPI_Status status[maxRequest];

	for (int i = 0; i < num_request; i++)
		MPI_Wait(&requests[i], &status[i]);

	num_request = 0;

}

void MPI_comm_strategy::reduceAll(double* one_, double* all_, const int size_)
{
	MPI_Allreduce(one_, all_, size_, MPI_DOUBLE,MPI_SUM,world);
}

void MPI_comm_strategy::reduceAll(int* one_, int* all_, const int size_)
{
	MPI_Allreduce(one_, all_, size_, MPI_INT, MPI_SUM, world);
}

std::unique_ptr<Comm_strategy> MPI_comm_strategy::split(const int maxRanks_)
{
	int newColor, newRank;

	if (rank < maxRanks_) {
		newColor = 0;
		newRank = this->rank;
	}
	else
	{
		newColor = 1;
		newRank = this->rank - maxRanks_;
	}
	

	MPI_Comm newComm;
	MPI_Comm_split(world, newColor, newRank, &newComm);

	std::unique_ptr<Comm_strategy> out = std::make_unique<MPI_comm_strategy>(newComm, newRank);
	return out;

}
