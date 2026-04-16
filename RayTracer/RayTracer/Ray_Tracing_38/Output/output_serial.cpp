#include "output_serial.h"


output_serial::output_serial(
	std::string _file_name,
	color_array* _colors,
	int _image_width, int _image_height,
	outputMode mode_) :
	output{_file_name,_colors,_image_width,_image_height,mode_}
{
	// if string is empty which is the case
	// for the constructor of the render_animation
	// do not open any new files
	if (mode == outputMode::P3)
		stream = std::make_unique<std::fstream>(file_name);
	else if (mode == outputMode::P6)
		stream = std::make_unique<std::fstream>(file_name, std::ios::binary | std::ios::trunc);
}

output_serial::output_serial(std::string _file_name,
	outputMode mode_) :
	output_serial{ _file_name, nullptr, 0, 0, mode_ } {
}


output_serial::output_serial(
	std::unique_ptr<std::iostream> _stream,
	color_array* _colors,
	int _image_width, int _image_height,
	outputMode mode_) :
	output{std::move(_stream),_colors,_image_width,_image_height,mode_}
{}

void output_serial::write_file()
{
	write_header();
	color_data** c_data = colors->return_array();


	if (mode == outputMode::P3) {
		for (int j = 0; j < image_height; j++)
		{
			for (int i = 0; i < image_width; i++)
			{
				*stream << c_data[i][j]; //This leads to strided access ==> might need to change the indexing to [height_index][width_index]
			}
		}
	}
	else if (mode == outputMode::P6)
	{
		for (int j = 0; j < image_height; j++)
		{
			for (int i = 0; i < image_width; i++)
			{
				color_array::write_binary(*stream, c_data[i][j]);
			}
		}
	}
}