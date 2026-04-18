#include "output_parallel.h"

#include <fstream>
#include "../Data/color_array.h"

output_parallel::output_parallel(
	std::string _file_name,
	color_array* _colors,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	output{_file_name,_colors,mode_},
	para{para_.get()}
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

	init();
}

output_parallel::output_parallel(
	std::string _file_name,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	output_parallel{_file_name,nullptr,para_,mode_}
{}


output_parallel::output_parallel(
	std::unique_ptr<std::iostream> _stream,
	color_array* _colors,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	output{std::move(_stream),_colors,mode_},
	para{para_.get()}
{
	init();
}


output_parallel::~output_parallel()
{}

void output_parallel::init()
{
	int image_width, image_height;
	colors->return_size(image_width, image_height);

	auto rank_config = para->return_rank_config();
	auto size_config = para->return_size_config();

	int widthPerRank = (image_width + size_config[0] - 1) / size_config[0];
	int heightPerRank = (image_height + size_config[1] - 1) / size_config[1];

	int widthMin = rank_config[0] * widthPerRank;
	// the max is not inclusive
	int widthMax = widthPerRank + widthMin;

	widthMin = widthMin < image_width ? widthMin : image_width - 1;
	// since the max is not inclusive, it can be equal to image_width
	widthMax = widthMax <= image_width ? widthMax : image_width;
	
	int heightMin = rank_config[1] * heightPerRank;
	int heightMax = heightPerRank + heightMin;

	heightMin = heightMin < image_height ? heightMin : image_height - 1;
	// since the max is not inclusove, it can be equal to the image_height
	heightMax = heightMax <= image_height ? heightMax : image_height;

	// widthMax is not inclusive
	int myWidth = widthMax - widthMin;

	// when the row range for this rank 
	// finishes being written the pointer is 
	// at the end of the current row range.
	// Thus the stride of image_width 
	// moves the pointer to the end of
	// the next row range.
	// So there is need to go back 
	// myWidth location.
	writeStride = 3*(image_width - myWidth);

	myWidthRange[0] = widthMin;
	myWidthRange[1] = widthMax;

	myHeightRange[0] = heightMin;
	myHeightRange[1] = heightMax;
}

void output_parallel::write_file()
{
	int image_width, image_height;
	colors->return_size(image_width, image_height);

	// getting the rank number
	int rank = para->return_rank();
	// the begining of the binary section
	std::streampos pos;
	// writing the header from the rank 0
	if (rank == 0) {
		pos = write_header();
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






