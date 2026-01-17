#pragma once

#include <array>
#include <vector>
#include <string>

using ARRAY_INT_3 = std::array<int, 3>;
using ARRAY_DATA_3 = std::array<std::vector<std::string>, 3>;
using DATA_STRUCT =
struct {
	ARRAY_INT_3 nums;
	ARRAY_DATA_3 data;
};
