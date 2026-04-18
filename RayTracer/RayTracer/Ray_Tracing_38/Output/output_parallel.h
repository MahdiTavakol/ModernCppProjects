#pragma once
#include "../Algorithms/parallel.h"
#include "output.h"

class output_parallel : public output {
public:
	output_parallel(std::string _file_name,
		color_array* _colors,
		int _image_width, int _image_height,
		std::unique_ptr<parallel>& para_,
		outputMode mode = outputMode::P6);
	output_parallel(
		std::string _file_name,
		std::unique_ptr<parallel>& para_,
		outputMode mode = outputMode::P6);
	output_parallel(
		std::unique_ptr<std::iostream> _stream,
		color_array* _colors,
		int _image_width, int _image_height,
		std::unique_ptr<parallel>& para_,
		outputMode mode = outputMode::P6);
	~output_parallel();

	void write_file() override;


private:
	void init();
	std::array<int, 2> myWidthRange = { 0,0 };
	std::array<int, 2> myHeightRange = { 0,0 };
	int writeStride = 0;
	parallel* para;


	void bcast_streampos(std::streampos& pos_);

};