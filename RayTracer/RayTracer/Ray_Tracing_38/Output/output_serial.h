#pragma once
#include "output.h"

class output_serial : public output {
public:
	output_serial(std::string _file_name, color_array* _colors,
		int _image_width, int _image_height,
		outputMode mode_ = outputMode::P3);
	output_serial(std::string _file_name,
		outputMode mode_ = outputMode::P3);
	output_serial(std::unique_ptr<std::ostream> _stream,
		color_array* _colors,
		int _image_width, int _image_height,
		outputMode mode_ = outputMode::P3
	);
};
