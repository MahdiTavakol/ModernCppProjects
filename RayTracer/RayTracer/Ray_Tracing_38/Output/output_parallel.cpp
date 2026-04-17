#include "output_parallel.h"

#include <fstream>
#include "../Data/color_array.h"

output_parallel::output_parallel(
	std::string _file_name,
	color_array* _colors,
	int _image_width, int _image_height,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	output{_file_name,_colors,_image_width,_image_height,mode_},
	para{para_.get()}
{
	// if string is empty which is the case
	// for the constructor of the render_animation
	// do not open any new files
	if (mode == outputMode::P3)
		throw std::invalid_argument("The text format is not supported for parallel writing");
	stream = std::make_unique<std::fstream>(file_name, std::ios::binary | std::ios::trunc);
}

output_parallel::output_parallel(
	std::string _file_name,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	output{_file_name,nullptr,0,0,mode_},
	para{para_.get()}
{}


output_parallel::output_parallel(
	std::unique_ptr<std::iostream> _stream,
	color_array* _colors,
	int _image_width, int _image_height,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	output{std::move(_stream),_colors,_image_width,_image_height,mode_},
	para{para_.get()}
{}


output_parallel::~output_parallel()
{}

void output_parallel::init()
{
	auto rank_config = para->return_rank_config();
	auto size_config = para->return_size_config();

	int widthPerRank = image_width / size_config[0];
	int heightPerRank = image_height / size_config[1];

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
	writeStride = image_width - myWidth;
}

void output_parallel::write_file()
{
	// writing the header from the rank 0
	write_header();
	// barrier
	para->barrier();
	// getting the begining of the binary section
	auto pos = return_binary_begin();
	// moving the begining of the binary section
	stream->seekp(pos);
	// going to the begining of the first row
	// of this rank.
	// Moving with respect to the current location 
	// which is the begining of the binary section
	stream->seekp(myWidthRange[0], std::ios::cur);
	// writing the data
	colors->write(*stream, mode, writeStride);
}






