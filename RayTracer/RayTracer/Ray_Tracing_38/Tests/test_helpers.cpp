#include "test_helpers.h"

void minRanksRequirement(std::unique_ptr<parallel>& para_, const std::array<int,2>& minSize_)
{
	int size = para_->return_size();
	int min = minSize_[0] * minSize_[1];
	std::string warningText = 
		std::string("This test requires at least ") + 
		std::to_string(minSize_[0]) +
		std::string("X") +
		std::to_string(minSize_[1]) +
		std::string(" ranks!");

	if (size < min)
		SKIP(warningText.c_str());
}

std::unique_ptr<parallel> skipExtraRanks(std::unique_ptr<parallel>& para_, const std::array<int,2>& maxSize_)
{
	auto rank = para_->return_rank();
	std::unique_ptr<parallel> new_para = para_->split(maxSize_);
	std::string warningText = 
		std::string("This test just uses the first ") +
		std::to_string(maxSize_[0]) + 
		std::string("X") +
		std::to_string(maxSize_[1]) +
		std::string(" ranks!\nSkipping this rank");
	if (rank >= maxSize_[0]*maxSize_[1])
		SKIP(warningText.c_str());
	return new_para;
}

void printRankZero(std::unique_ptr<parallel>& para_, const std::string text)
{
	auto rank = para_->return_rank_config();

	if (rank[0] == 0 && rank[1] == 0)
		std::cout << text << std::endl;
}