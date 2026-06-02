#include "simpleComm.h"

simpleComm::simpleComm(std::array<int, 2> rank_config_, std::array<int, 2> size_config_):
	communicator()
{
	size_config = size_config_;
	rank_config = rank_config_;
}