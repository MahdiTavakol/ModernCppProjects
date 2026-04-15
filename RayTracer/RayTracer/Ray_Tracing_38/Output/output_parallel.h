#pragma once
#include "../Algorithms/parallel.h"

class output_parallel {
public:
	output_parallel(std::string _file_name, 
		color_array* _colors,
		int _image_width, int _image_height,
		std::unique_ptr<parallel>& para_);
	output_parallel(
		std::string _file_name, 
		std::unique_ptr<parallel>& para_);
	output_parallel(
		std::unique_ptr<std::ostream> _stream, 
		color_array* _colors,
		int _image_width, int _image_height, 
		std::unique_ptr<parallel>& para_);
	~output_parallel();
	void write_file();
	void reset(color_array* _colors, const int _image_width, const int _image_height);
	void open_new_file(std::string _file_name);
	std::unique_ptr<std::ostream> return_stream();

protected:
	std::string file_name;
	std::unique_ptr<std::ostream> stream;
	std::ofstream* file;
	color_array* colors;
	int image_width, image_height;

private:
	parallel* para;

};