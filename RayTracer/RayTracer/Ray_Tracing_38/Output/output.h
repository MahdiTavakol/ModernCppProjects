#pragma once

#include <fstream>

#include "../Shared/rtweekend.h"

#include "../Data/color_array.h"
#include "../Algorithms/image.h"
#include "../Input/settings.h"
#include "../Input/output_settings.h"



class output {
public:
	output(settings* out_settings,
		   communicator* para_);
	output(std::string _file_name, 
		   std::unique_ptr<image>&& img_,
		   std::unique_ptr<communicator>& para_,
		   outputMode mode_ );
	output(std::string _file_name,
		   std::unique_ptr<communicator>& para_,
		   outputMode mode_);
	output(std::unique_ptr<std::iostream> _stream,
		   std::unique_ptr<image>&& img_,
		   std::unique_ptr<communicator>& para_,
		   outputMode mode_);
	virtual ~output();

	virtual void write_file() = 0;
	void reset_image(std::unique_ptr<image>&& img_);
	void open_new_file(std::string _file_name);
	void reset_image(std::string file_name_, std::unique_ptr<image>&& img_);
	std::unique_ptr<std::iostream> return_stream();

protected:
	std::streampos return_binary_begin();
	static void remove_file(const std::string& fileName_);
	std::streampos write_header(const int& width_, const int& height_);
	std::streampos write_header();
	outputMode outMode = outputMode::P3;
	std::string file_name;
	// I want it to be iostream so that output_parallel can read it 
	// to get the begining of the binary section
	std::unique_ptr<std::iostream> stream;
	std::fstream* file = nullptr;

	std::unique_ptr<image> img;

	// running strategy
	communicator* para;
};

