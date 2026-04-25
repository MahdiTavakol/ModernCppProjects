#pragma once
#include "output.h"

class output_serial : public output {
public:
	output_serial(
		settings* out_settings,
		communicator* para_);
	output_serial(std::string _file_name,
		std::unique_ptr<image>&& img_,
		std::unique_ptr<communicator>& para_,
		outputMode mode_ = outputMode::P3);
	output_serial(std::string _file_name,
		std::unique_ptr<communicator>& para_,
		outputMode mode_ = outputMode::P3);
	output_serial(std::unique_ptr<std::iostream> _stream,
		std::unique_ptr<image>&& img_,
		std::unique_ptr<communicator>& para_,
		outputMode mode_ = outputMode::P3
	);

	void write_file() override;
};
