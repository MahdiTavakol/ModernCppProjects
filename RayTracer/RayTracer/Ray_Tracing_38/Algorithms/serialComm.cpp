#include "serialComm.h"

serialComm::serialComm() :
    communicator{}
{
}

serialComm::~serialComm()
{
}

void serialComm::gather(int** one_, int** one_all_, const int& width_per_rank_, const int& height_per_rank_) const
{
    // switching the pointers 
	int** dummy = one_all_;
	one_all_ = one_;
	one_ = dummy;
}

std::unique_ptr<communicator> serialComm::split(const std::array<int, 2>& maxRanks_) const
{
	// creating another serial communicator does not mean anything since there is only one rank,
    // so just return a new serial communicator
    auto out = std::make_unique<serialComm>();

    return out;
}


void serialComm::gather(color_data** one_,
    color_data** one_all_,
    const int& width_per_rank_,
    const int& height_per_rank_) const
{
	color_data** dummy = one_all_;
	one_all_ = one_;
	one_ = dummy;
}

void serialComm::gather(color_data* one_, color_data* one_all_, const int& num_data_) const
{
	color_data* dummy = one_all_;
	one_all_ = one_;
	one_ = dummy;
}

void serialComm::gather(
    std::unique_ptr<color_array>& one_,
    std::unique_ptr<color_array>& one_all_,
    std::array<int, 2>& size_per_rank_,
    std::array<int, 2>& size_) const
{
    one_.swap(one_all_);
}


void serialComm::barrier() const {
	//nothing to do here since there is only one rank
}

void serialComm::bcast(void* buff_, int nBytes_, int root_) const
{
	// nothing to do here since there is only one rank
}


