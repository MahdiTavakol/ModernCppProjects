#pragma once

#include <fstream>

#include "../Shared/rtweekend.h"

#include "../Data/color_array.h"



class output {
public:
	output(std::string _file_name, color_array* _colors, 
		   int _image_width, int _image_height,
		   outputMode mode_ );
	output(std::string _file_name,
		   outputMode mode_);
	output(std::unique_ptr<std::ostream> _stream, 
		   color_array* _colors,
		   int _image_width, int _image_height,
		   outputMode mode_
		);
	virtual ~output();

	virtual void write_file() = 0;
	void reset(color_array* _colors, const int _image_width, const int _image_height);
	void open_new_file(std::string _file_name);
	std::unique_ptr<std::ostream> return_stream();

protected:
	std::streampos return_binary_begin();
	void write_header();
	outputMode mode = outputMode::P3;
	std::string file_name;
	// I want it to be iostream so that output_parallel can read it 
	// to get the begining of the binary section
	std::unique_ptr<std::iostream> stream;
	std::ofstream* file;
	color_array* colors;
	int image_width, image_height;
};

