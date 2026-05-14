#include "output.h"

#include <fstream>
#include <algorithm>
#include "../Data/color_array.h"

output::output(settings* out_settings_,
	communicator* para_) :
	para{ para_ }
{
	// checking the setting type
	output_settings* sett = dynamic_cast<output_settings*>(out_settings_);
	if (!sett)
		throw std::invalid_argument("Wrong settings object");

	file_name = sett->return_file_name();
	outMode = sett->return_outputMode();

	size = sett->return_size();
}


output::output(settings* out_settings_,
			   std::unique_ptr<image>&& img_,
	           communicator* para_):
	para{para_},
	img{std::move(img_)}
{
	// checking the setting type
	output_settings* sett = dynamic_cast<output_settings*>(out_settings_);
	if (!sett)
		throw std::invalid_argument("Wrong settings object");

	file_name = sett->return_file_name();
	outMode = sett->return_outputMode();

	size = sett->return_size();
}

output::output(
	std::string _file_name, 
	std::unique_ptr<image>&& img_,
	std::unique_ptr<communicator>& para_,
	outputMode outMode_) :
	outMode{outMode_},
	file_name{ _file_name }, 
	img{std::move(img_)},
	para{ para_.get() }
{}

output::output(std::string _file_name,
			   std::unique_ptr<communicator>& para_,
	           outputMode mode_) :
	output{ _file_name, nullptr, para_, mode_} {}

output::output(
	std::unique_ptr<std::iostream> _stream,
	std::unique_ptr<image>&& img_,
	std::unique_ptr<communicator>& para_,
	outputMode outMode_) :
	outMode{ outMode_ },
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

void output::reset_image(std::unique_ptr<image>&& img_)
{
	img = std::move(img_);
}

void output::open_new_file(std::string _file_name)
{
	file_name = _file_name;
	if (file && file->is_open())
		file->close();
	if (outMode == outputMode::P3)
		stream = std::make_unique<std::fstream>(file_name, std::ios::out);
	else if (outMode == outputMode::P6)
	{
		stream = std::make_unique<std::fstream>(file_name, std::ios::in | std::ios::out | std::ios::binary);
		file = dynamic_cast<std::fstream*>(stream.get());
	}
	else
		std::cerr << "Wrong mode" << std::endl;
	file = dynamic_cast<std::fstream*>(stream.get());
	if (!file->is_open())
		std::cerr << "Cannot open the file " << file_name << std::endl << "That is all we know at the moment!" << std::endl;
}


void output::reset_image(std::string file_name_, std::unique_ptr<image>&& img_)
{
	open_new_file(file_name_);
	img = std::move(img_);
}

void output::remove_file(const std::string& fileName_)
{
	std::remove(fileName_.c_str());
}

std::unique_ptr<image> output::return_image()
{
	return std::move(img);
}

image* output::return_image_ptr()
{
	return img.get();
}

std::unique_ptr<std::iostream> output::return_stream()
{
	if (stream == nullptr)
		std::cerr << "The stream has either been already returned or it is null" << std::endl;
	return std::move(stream);
}

std::streampos output::write_header(const int& width_, const int& height_)
{
	if (outMode == outputMode::P3)
	{
		*stream << "P3\n";
	}
	else if (outMode == outputMode::P6)
	{
		*stream << "P6\n";
	}
	*stream << width_ << " " << height_ << "\n255\n";

	auto pos = stream->tellp();
	binary_pos = pos;

	return pos;
}


std::streampos output::return_binary_begin()
{
	return binary_pos;
}

void output::setup()
{
	// removing the file if it exist
	remove_file(file_name);
	// creating the file
	std::ofstream temp{ file_name };
	temp.close();

	// reopening the file in a proper mode
	open_new_file(file_name);

	// writing the header
	int image_width, image_height;
	img->returnSize(image_width, image_height);

	write_header(image_width, image_height);
}

void output::write_file(image* img_, std::streampos pos_)
{
	// getting the image properties
	int image_width, image_height;
	std::array<int, 2> myWidthRange, myHeightRange;
	int writeStride;
	img_->returnSize(image_width, image_height);
	img_->returnRange(myWidthRange, myHeightRange);
	// widthMax is not inclusives
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


	// moving the begining of the binary section
	stream->seekp(pos_);
	// going to the begining of the first row
	// of this rank.
	// Moving with respect to the current location 
	// which is the begining of the binary section

	int pos0 = 3 * (myHeightRange[0] * image_width + myWidthRange[0]);

	stream->seekp(pos0, std::ios::cur);
	// getting a pointer to the color_array object of the img
	auto* colors = img_->array();
	// writing the data
	colors->write(*stream, outMode, writeStride);
}

std::string output::return_file_name()
{
	return file_name;
}


