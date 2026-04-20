#include "output_parallel.h"

#include <fstream>
#include "../Data/color_array.h"

output_parallel::output_parallel(
	std::string _file_name,
	std::unique_ptr<image>&& img_,
	std::unique_ptr<communicator>& para_,
	outputMode mode_) :
	output{_file_name,std::move(img_),para_,mode_}
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
		remove_file(_file_name);
		std::ofstream temp{ _file_name };
	}
	// barrier 
	para->barrier();
	// if string is empty which is the case
	// for the constructor of the render_animation
	// do not open any new files
	if (mode == outputMode::P3)
		throw std::invalid_argument("The text format is not supported for parallel writing");
	open_new_file(_file_name);

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


void output_parallel::write_file()
{
	// getting the image properties
	int image_width, image_height;
	std::array<int, 2> myWidthRange, myHeightRange;
	int writeStride;
	img->returnSize(image_width, image_height);
	img->returnRange(myWidthRange, myHeightRange);
	// widthMax is not inclusive
	int myWidth = myWidthRange[1] - myWidthRange[0];
	// when the row range for this rank 
	// finishes being written the pointer is 
	// at the end of the current row range.
	// Thus the stride of image_width 
	// moves the pointer to the end of
	// the next row range.
	// So there is need to go back 
	// myWidth location.
	writeStride = 3 * (image_width - myWidth);
	// getting the rank number
	int rank = para->return_rank();
	// the begining of the binary section
	std::streampos pos;
	// writing the header from the rank 0
	if (rank == 0) {
		pos = write_header(image_width,image_height);
	}

	// broadcasting the begining of the binary section of the file
	bcast_streampos(pos);

	// moving the begining of the binary section
	stream->seekp(pos);
	// going to the begining of the first row
	// of this rank.
	// Moving with respect to the current location 
	// which is the begining of the binary section

	int pos0 = 3 * (myHeightRange[0] * image_width + myWidthRange[0]);
	
	stream->seekp(pos0, std::ios::cur);
	// getting a pointer to the color_array object of the img
	auto* colors = img->array();
	// writing the data
	colors->write(*stream, mode, writeStride);
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






