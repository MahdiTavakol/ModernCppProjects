#include "test.hpp"

void minRanksRequirement(std::unique_ptr<Comm_strategy>& comm_strategy_, const int minSize_)
{
	int size = comm_strategy_->getSize();
	std::string warningText = std::string("This test requires at least ") + std::to_string(minSize_) + std::string(" ranks!");
	if (size < minSize_)
		SKIP(warningText.c_str());
}

void skipExtraRanks(std::unique_ptr<Comm_strategy>& comm_strategy_, const int maxSize_)
{
	int rank = comm_strategy_->getRank();
	std::string warningText = std::string("This test just uses the first ") +
		std::to_string(maxSize_) + std::string(" ranks!\nSkipping this rank");
	if (rank >= maxSize_)
		SKIP(warningText.c_str());
}