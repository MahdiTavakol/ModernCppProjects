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


	void setup() override;

private:
	std::mutex file_mut;
	std::unique_ptr<std::once_flag> file_set_ptr;
	std::unique_ptr<std::once_flag> header_set_ptr;
	std::condition_variable file_cond;
	std::atomic<std::streampos> pos;


};
