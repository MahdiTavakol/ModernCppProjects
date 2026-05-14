#include "output_parallel.h"

#include <fstream>
#include "../Data/color_array.h"

output_parallel::output_parallel(
	settings* out_settings,
	communicator* para_) :
	output{ out_settings,para_ }
{
}

output_parallel::output_parallel(
	settings* out_settings,
	std::unique_ptr<image>&& img_,
	communicator* para_):
	output{out_settings,std::move(img_),para_}
{ 
}

output_parallel::output_parallel(
	std::string _file_name,
	std::unique_ptr<image>&& img_,
	std::unique_ptr<communicator>& para_,
	outputMode mode_) :
	output{_file_name,std::move(img_),para_,mode_}
{
}

output_parallel::output_parallel(
	std::string _file_name,
	std::unique_ptr<communicator>& para_,
	outputMode mode_) :
	output_parallel{_file_name,nullptr,para_,mode_}
{}


output_parallel::output_parallel(
	std::unique_ptr<std::iostream> _stream,
	std::unique_ptr<image>&& img_,
	std::unique_ptr<communicator>& para_,
	outputMode mode_) :
	output{std::move(_stream),std::move(img_),para_,mode_}
{}


output_parallel::~output_parallel()
{}

void output_parallel::setup()
{
	/*
	 * First, the rank 0 checks if the file exists
	 * it removes that file.
	 * Every rank needs to wait for the check to be finished before
	 * proceeding, otherwise the file opened by another rank
	 * will be deleted by the rank 0!
	 * Then, each rank opens the file in the non-trunk mode
	 * as they want to write together and it is possible
	 * that another rank has already created the file.
	 * Which rank first creates the file does not matter.
	 */
	 /*
	 * Since out ios mode is in and out
	 * the file needs to exist when it is opened.
	 * That is the reason why here the file is created.
	 */
	int rank = para->return_rank();
	if (rank == 0) {
		remove_file(file_name);
		std::ofstream temp{file_name };
	}
	// barrier 
	para->barrier();

	if (outMode == outputMode::P3)
		throw std::invalid_argument("The text format is not supported for parallel writing");

	open_new_file(file_name);

	if (!img)
		return;

	int image_width, image_height;
	img->returnSize(image_width, image_height);

	if (rank == 0) {
		write_header(image_width, image_height);
	}

	// broadcasting the begining of the binary section of the file
	bcast_streampos(binary_pos);
}


void output_parallel::bcast_streampos(std::streampos& pos_)
{
	// std::streamoff is just a number
	std::streamoff offset = pos_;
	// npos bytes
	int nBytes = sizeof(offset);
	// temp void* to contain the binPos
	// I hope the size of binPos is the same in all the ranks!
	void* temp = static_cast<void*>(&offset);
	// bcasting it from the rank 0
	para->bcast(temp, nBytes, 0);
	// convering to the binPos
	pos_ = offset;
}







