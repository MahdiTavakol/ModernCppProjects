#include "output.h"

#include <fstream>
#include <algorithm>
#include "../Data/color_array.h"

output::output(
	std::string _file_name, 
	color_array* _colors, 
	int _image_width, int _image_height,
	outputMode mode_) :
	mode{mode_},
	file_name{ _file_name }, 
	colors{ _colors },
	image_width{ _image_width }, image_height{ _image_height }
{}

output::output(std::string _file_name,
	           outputMode mode_) :
	output{ _file_name, nullptr, 0, 0, mode_} {}


output::output(std::unique_ptr<std::iostream> _stream, 
	           color_array* _colors, 
	           int _image_width, int _image_height,
	           outputMode mode_):
	mode{mode_},
	stream{std::move(_stream)}, colors{_colors},
	image_width{_image_width}, image_height{_image_height}
{}

	
output::~output()
{
	// checking if it is a full
	file = dynamic_cast<std::fstream*>(stream.get());
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
	if (mode == outputMode::P3)
		stream = std::make_unique<std::fstream>(file_name, std::ios::out);
	else if (mode == outputMode::P6)
		stream = std::make_unique<std::fstream>(file_name,std::ios::in | std::ios::out | std::ios::binary);
	else
		std::cerr << "Wrong mode" << std::endl;
	file = dynamic_cast<std::fstream*>(stream.get());
	if (!file->is_open())
		std::cerr << "Cannot open the file " << file_name << std::endl << "That is all we know at the moment!" << std::endl;
}

void output::remove_file(const std::string& fileName_)
{
	std::remove(fileName_.c_str());
}


std::unique_ptr<std::iostream> output::return_stream()
{
	if (stream == nullptr)
		std::cerr << "The stream has either been already returned or it is null" << std::endl;
	return std::move(stream);
}

std::streampos output::write_header()
{
	if (mode == outputMode::P3)
	{
		*stream << "P3\n";
	}
	else if (mode == outputMode::P6)
	{
		*stream << "P6\n";
	}
	*stream << image_width << " " << image_height << "\n255\n";

	auto pos = stream->tellp();
	return pos;
}

std::streampos output::return_binary_begin()
{
	std::string type;
	int width, height, maxvel;

	*stream >> type >> width >> height >> maxvel;

	// seeking to the begining of the binary section.
	stream->get();


	// getting the g position (read)
	std::streampos pos = stream->tellg();

	// returning the pos
	return pos;
}


