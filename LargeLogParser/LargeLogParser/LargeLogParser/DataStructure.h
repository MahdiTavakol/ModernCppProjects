#pragma once

#include <array>
#include <vector>
#include <string>

class DataStructure {
public:
	DataStructure(): numLines{0}
	{
		nums[0] = nums[1] = nums[2] = 0;
	}

	void operator()(int& num_lines_, int& num_errors_, int& num_warns_, int& num_infos_) const;

	void operator()(std::vector<std::string>& errors_, 
		           std::vector<std::string>& warns_, 
		           std::vector<std::string>& infos_);

	int& returnNumLines() { return numLines; }
	std::array<int, 3>& returnNums() { return nums; }
	std::array<std::vector<std::string>,3>& returnData() { return data; }
	
	void append(DataStructure&& rhs_);


protected:
	int numLines;
	std::array<int, 3> nums;
	std::array<std::vector<std::string>, 3> data;
};
