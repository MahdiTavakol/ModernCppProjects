#include "output_serial.h"

output_serial::output_serial(
	settings* out_settings,
	communicator* para_) :
	output{ out_settings,para_ }
{}

output_serial::output_serial(
	std::string _file_name,
	std::unique_ptr<image>&& img_,
	std::unique_ptr<communicator>& para_,
	outputMode mode_) :
	output{_file_name,std::move(img_),para_,mode_}
{
	// if string is empty which is the case
	// for the constructor of the render_animation
	// do not open any new files
	open_new_file(_file_name);
}

output_serial::output_serial(
	std::string _file_name,
	std::unique_ptr<communicator>& para_,
	outputMode mode_) :
	output_serial{ _file_name, nullptr,para_, mode_ } {
}


output_serial::output_serial(
	std::unique_ptr<std::iostream> _stream,
	std::unique_ptr<image>&& img_,
	std::unique_ptr<communicator>& para_,
	outputMode mode_) :
	output{std::move(_stream),std::move(img_),para_,mode_}
{}

void output_serial::write_file()
{
	// if the image is writting in a parallel manner,
	// the color_array needs to be gathered first and then written by the rank 0.
	std::unique_ptr<image> img_all;
	// the parallel class is written the way
	// that if the number of ranks is one, the gather function just copies the data from one_ to one_all.
	image::gather(img, img_all, para);
	// swapping the img with the gathered img
	img.swap(img_all);
	// getting the rank
	int rank = para->return_rank();
	// writing the file only in the rank 0
	if (rank == 0) {
		// writting the header info
		write_header();
		// getting a pointer to the color_array object of the image
		auto* colors = img->array();
		// for serial writting the stride is zero
		colors->write(*stream, outMode, 0);
	}
}