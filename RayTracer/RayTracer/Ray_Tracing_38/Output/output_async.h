#pragma once
#include "output.h"
#include <thread>


class output_async : public output
{
public:
	output_async(
		settings* out_settings,
		communicator* para_);
	output_async(
		settings* out_settings,
		std::unique_ptr<image>&& img_,
		communicator* para_);
	~output_async();


	//void setup() override;

private:
	std::mutex file_mut;
	std::atomic<std::streampos> pos;


};