#pragma once
#include "output.h"

class output_serial : public output {
public:
	output_serial(
		settings* out_settings,
		communicator* para_);
	output_serial(std::string _file_name,
		communicator* para_,
		outputMode mode_ = outputMode::P3);
	output_serial(std::unique_ptr<std::iostream> _stream,
		communicator* para_,
		outputMode mode_ = outputMode::P3
	);

	void write_file(image* img_, std::streampos pos_) override;

};
