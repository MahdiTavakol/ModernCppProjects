#pragma once
#include "../Algorithms/communicator.h"
#include "output.h"

class output_parallel : public output {
public:
	output_parallel(std::string _file_name,
		std::unique_ptr<image>&& img_,
		std::unique_ptr<communicator>& para_,
		outputMode mode = outputMode::P6);
	output_parallel(
		std::string _file_name,
		std::unique_ptr<communicator>& para_,
		outputMode mode = outputMode::P6);
	output_parallel(
		std::unique_ptr<std::iostream> _stream,
		std::unique_ptr<image>&& img_,
		std::unique_ptr<communicator>& para_,
		outputMode mode = outputMode::P6);
	~output_parallel();

	void write_file() override;


private:

	void bcast_streampos(std::streampos& pos_);

};