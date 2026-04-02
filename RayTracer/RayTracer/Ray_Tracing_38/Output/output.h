#pragma once

#include <fstream>

#include "../Shared/rtweekend.h"

#include "../Data/color_array.h"

class output {
public:
	output(std::string _file_name, color_array* _colors, 
		   int _image_width, int _image_height);
	output(std::string _file_name);
	output(std::unique_ptr<std::ostream> _stream, color_array* _colors,
		   int _image_width, int _image_height);
	~output();
	void write_file();
	void reset(color_array* _colors, const int _image_width, const int _image_height);
	void open_new_file(std::string _file_name);
	std::unique_ptr<std::ostream> return_stream();

private:
	std::string file_name;
	std::unique_ptr<std::ostream> stream;
	std::ofstream* file;
	color_array* colors;
	int image_width, image_height;
};

