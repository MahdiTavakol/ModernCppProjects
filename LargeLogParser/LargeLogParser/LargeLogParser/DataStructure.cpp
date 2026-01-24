#include "DataStructure.h"
#include <exception>

void DataStructure::operator()(std::array<int,4>& nums_) const
{
	nums_[0] = numLines;
	nums_[1] = nums[0];
	nums_[2] = nums[1];
	nums_[3] = nums[2];
}

void DataStructure::operator()(std::vector<std::string>& errors_,
	std::vector<std::string>& warns_,
	std::vector<std::string>& infos_)
{
	if (nums[0] == 0 || nums[1] == 0 || nums[2] == 0)
		throw std::runtime_error("The data has already returned!");
	errors_ = std::move(data[0]);
	warns_ = std::move(data[1]);
	infos_ = std::move(data[2]);
	nums[0] = nums[1] = nums[2] = 0;
}

void DataStructure::append(DataStructure&& rhs_) {
	std::array<int, 3> numsRhs = rhs_.returnNums();
	std::array<std::vector<std::string>, 3> dataRhs = rhs_.returnData();
	nums[0] += numsRhs[0];
	nums[1] += numsRhs[1];
	nums[2] += numsRhs[2];
	data[0].insert(data[0].end(), dataRhs[0].begin(), dataRhs[0].end());
	data[1].insert(data[1].end(), dataRhs[1].begin(), dataRhs[1].end());
	data[2].insert(data[2].end(), dataRhs[2].begin(), dataRhs[2].end());
	numLines += rhs_.numLines;
}