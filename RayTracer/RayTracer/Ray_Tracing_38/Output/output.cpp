#include "output.h"

#include <fstream>
#include "../Data/color_array.h"

output::output(std::string _file_name, color_array* _colors, int _image_width, int _image_height) :
	file_name(_file_name), colors(_colors), image_width(_image_width), image_height(_image_height)
{
	// if string is empty which is the case
	// for the constructor of the render_animation
	// do not open any new files
	stream = std::make_unique<std::ofstream>(file_name);
}

output::output(std::string _file_name) :
	output{ _file_name, nullptr, 0, 0 } {}


output::output(std::unique_ptr<std::ostream> _stream, color_array* _colors, 
	           int _image_width, int _image_height):
	stream{std::move(_stream)}, colors{_colors},
	image_width{_image_width}, image_height{_image_height}
{}

	
output::~output()
{
	// checking if it is a full
	file = dynamic_cast<std::ofstream*>(stream.get());
	if (file && file->is_open())
	{
		file->close();
	}
}

void output::reset(color_array* _colors, const int _image_width, const int _image_height)
{
	this->colors = _colors;
	this->image_width = _image_width;
	this->image_height = _image_height;
}

void output::open_new_file(std::string _file_name)
{
	file_name = _file_name;
	if (file && file->is_open())
		file->close();
	stream = std::make_unique<std::ofstream>(file_name);
	file = dynamic_cast<std::ofstream*>(stream.get());
	if (!file->is_open())
		std::cerr << "Cannot open the file " << file_name << std::endl << "That is all we know at the moment!" << std::endl;
}

void output::write_file()
{
	*stream << "P3\n" << image_width << " " << image_height << "\n255\n";
	color_data** c_data = colors->return_array();

	for (int j = 0; j < image_height; j++)
	{
		for (int i = 0; i < image_width; i++)
		{
			*stream << c_data[i][j]; //This leads to strided access ==> might need to change the indexing to [height_index][width_index]
		}
	}
}


std::unique_ptr<std::ostream> output::return_stream()
{
	if (stream == nullptr)
		std::cerr << "The stream has either been already returned or it is null" << std::endl;
	return std::move(stream);
}


