#include "test_helpers.h"

void minRanksRequirement(std::unique_ptr<parallel>& para_, const std::array<int,2>& minSize_)
{
	auto size = para_->return_size_config();
	std::string warningText = 
		std::string("This test requires at least ") + 
		std::to_string(minSize_[0]) +
		std::string("X") +
		std::to_string(minSize_[1]) +
		std::string(" ranks!");

	if (size < minSize_)
		SKIP(warningText.c_str());
}

std::unique_ptr<parallel> skipExtraRanks(std::unique_ptr<parallel>& para_, const std::array<int,2>& maxSize_)
{
	auto rank = para_->return_rank_config();
	std::unique_ptr<parallel> new_para = para_->split(maxSize_);
	std::string warningText = 
		std::string("This test just uses the first ") +
		std::to_string(maxSize_[0]) + 
		std::string("X") +
		std::to_string(maxSize_[1]) +
		std::string(" ranks!\nSkipping this rank");
	if (rank >= maxSize_)
		SKIP(warningText.c_str());
	return new_para;
}

void printRankZero(std::unique_ptr<parallel>& para_, const std::string text)
{
	auto rank = para_->return_rank_config();

	if (rank[0] == 0 && rank[1] == 0)
		std::cout << text << std::endl;
}