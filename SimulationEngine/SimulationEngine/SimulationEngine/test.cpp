#include "test.hpp"
#include "Comm_strategy.h"
#include <mpi.h>

void minRanksRequirement(std::unique_ptr<Comm_strategy>& comm_strategy_, const int minSize_)
{
	int size = comm_strategy_->getSize();
	std::string warningText = std::string("This test requires at least ") + std::to_string(minSize_) + std::string(" ranks!");
	if (size < minSize_)
		SKIP(warningText.c_str());
}

std::unique_ptr<Comm_strategy> skipExtraRanks(std::unique_ptr<Comm_strategy>& comm_strategy_, const int maxSize_)
{
	int rank = comm_strategy_->getRank();
	std::unique_ptr<Comm_strategy> new_comm_strategy = comm_strategy_->split(maxSize_);
	std::string warningText = std::string("This test just uses the first ") +
		std::to_string(maxSize_) + std::string(" ranks!\nSkipping this rank");
	if (rank >= maxSize_)
		SKIP(warningText.c_str());
	return new_comm_strategy;
}

void printRankZero(const std::string text)
{
	int rank = -1;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
		std::cout << text << std::endl;
}