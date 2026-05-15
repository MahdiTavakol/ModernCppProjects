#pragma once
#include "../Algorithms/communicator.h"
#include "output.h"

class output_parallel : public output {
public:
	output_parallel(
		settings* out_settings,
		communicator* para_);
	output_parallel(
		std::string _file_name,
		communicator* para_,
		outputMode mode = outputMode::P6);
	output_parallel(
		std::unique_ptr<std::iostream> _stream,
		communicator* para_,
		outputMode mode = outputMode::P6);
	~output_parallel();



protected:
	virtual void setup(image* img_) override;

private:

	void bcast_streampos(std::streampos& pos_);

};