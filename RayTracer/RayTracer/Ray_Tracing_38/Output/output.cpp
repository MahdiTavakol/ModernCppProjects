#include "output.h"

#include <fstream>
#include <algorithm>
#include "../Data/color_array.h"

output::output(
	std::string _file_name, 
	std::unique_ptr<image>&& img_,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	mode{mode_},
	file_name{ _file_name }, 
	img{std::move(img_)},
	para{ para_.get() }
{}

output::output(std::string _file_name,
			   std::unique_ptr<parallel>& para_,
	           outputMode mode_) :
	output{ _file_name, nullptr, para_, mode_} {}

output::output(
	std::unique_ptr<std::iostream> _stream,
	std::unique_ptr<image>&& img_,
	std::unique_ptr<parallel>& para_,
	outputMode mode_) :
	mode{ mode_ },
	stream{ std::move(_stream)},
	img{ std::move(img_) },
	para{ para_.get() }
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

void output::reset(std::unique_ptr<image>&& img_)
{
	img = std::move(img_);
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

std::streampos output::write_header(const int& width_, const int& height_)
{
	if (mode == outputMode::P3)
	{
		*stream << "P3\n";
	}
	else if (mode == outputMode::P6)
	{
		*stream << "P6\n";
	}
	*stream << width_ << " " << height_ << "\n255\n";

	auto pos = stream->tellp();
	return pos;
}

std::streampos output::write_header()
{
	int image_width, image_height;
	img->returnSize(image_width, image_height);
	return this->write_header(image_width, image_height);
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


