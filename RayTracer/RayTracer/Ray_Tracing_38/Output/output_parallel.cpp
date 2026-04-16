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
	int widthMax = widthPerRank + widthMin;

	widthMin = widthMin < image_width ? widthMin : image_width - 1;
	widthMax = widthMax < image_width ? widthMax : image_width - 1;
	
	int heightMin = rank_config[1] * heightPerRank;
	int heightMax = heightPerRank + heightMin;

	heightMin = heightMin < image_height ? heightMin : image_height - 1;
	heightMax = heightMax < image_height ? heightMax : image_height - 1;


	// setting the location for each height value
	loc4Height.reserve(heightPerRank);

	for (int i = heightMin; i < heightMax; i++)
	{
		int loc = i * image_width + widthMin;
		loc4Height.push_back(loc);
	}
}

void output_parallel::write_file()
{
	// writing the header from the rank 0
	write_header();

	// barrier
	para->barrier();
	// getting the begining of the binary section
	auto pos = return_binary_begin();
	// moving the write location to the pos
	stream->seekp(pos);


	color_data** c_data = colors->return_array();

	for (int j = myHeightRange[0]; j < myHeightRange[1]; j++)
	{
		stream->seekp(loc4Height[j - myHeightRange[0]]);
		for (int i = myWidthRange[0]; i < myWidthRange[1]; i++)
		{
			color_array::write_binary(*stream, c_data[i][j]);
		}
	}
}






