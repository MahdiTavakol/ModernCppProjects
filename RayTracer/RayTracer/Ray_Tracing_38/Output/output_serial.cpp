#include "output_serial.h"


output_serial::output_serial(
	std::string _file_name,
	color_array* _colors,
	outputMode mode_) :
	output{_file_name,_colors,mode_}
{
	// if string is empty which is the case
	// for the constructor of the render_animation
	// do not open any new files
	open_new_file(_file_name);
}

output_serial::output_serial(std::string _file_name,
	outputMode mode_) :
	output_serial{ _file_name, nullptr, mode_ } {
}


output_serial::output_serial(
	std::unique_ptr<std::iostream> _stream,
	color_array* _colors,
	outputMode mode_) :
	output{std::move(_stream),_colors,mode_}
{}

void output_serial::write_file()
{
	// writting the header info
	write_header();

	// for serial writting the stride is zero
	colors->write(*stream, mode, 0);
}