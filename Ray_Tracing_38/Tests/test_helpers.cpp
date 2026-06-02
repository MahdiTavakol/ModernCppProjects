#include "test_helpers.h"

void minRanksRequirement(std::unique_ptr<communicator>& para_, const std::array<int,2>& minSize_)
{
	int size = para_->return_size();
	int min = minSize_[0] * minSize_[1];
	std::string warningText = 
		std::string("This test requires at least ") + 
		std::to_string(minSize_[0]) +
		std::string("X") +
		std::to_string(minSize_[1]) +
		std::string(" ranks!");

}

std::unique_ptr<communicator> skipExtraRanks(std::unique_ptr<communicator>& para_, const std::array<int,2>& maxSize_)
{
	auto rank = para_->return_rank();
	std::unique_ptr<communicator> new_para = para_->split(maxSize_);
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

void printRankZero(std::unique_ptr<communicator>& para_, const std::string text)
{
	int rank = para_->return_rank();

	if (rank == 0)
		std::cout << text << std::endl;
}

// allocater and deallocator lambda functions
void allocate(color_data**& data_, int width_, int height_)
{
	color_data* temp;
	temp = new color_data[width_ * height_];
	data_ = new color_data * [width_];
	for (int i = 0; i < width_; i++)
		data_[i] = &temp[i * height_];
};

void deallocate(color_data**& data_)
{
	delete[] data_[0];
	delete[] data_;
};