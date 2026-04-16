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
	stream = std::make_unique<std::ofstream>(file_name);
}

output_parallel::output_parallel(
	std::string _file_name,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	output{_file_name,nullptr,0,0,mode_},
	para{para_.get()}
{}


output_parallel::output_parallel(
	std::unique_ptr<std::ostream> _stream,
	color_array* _colors,
	int _image_width, int _image_height,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	output{std::move(_stream),_colors,_image_width,_image_height,mode_},
	para{para_.get()}
{}


output_parallel::~output_parallel()
{}

void output_parallel::reset_myRange()
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


	std::string type;
	int width, height, maxvel;

	*stream >> type >> width >> height >> maxvel;

	// seeking to the begining of the binary section.
	*stream.get();
}






