#pragma once
#include "output.h"
#include <thread>
#include <queue>


class output_async : public output
{
public:
	output_async(
		settings* out_settings_,
		communicator* para_);
	output_async(
		settings* out_settings,
		std::unique_ptr<image>&& img_,
		communicator* para_);
	~output_async();


	void setup() override;


	bool try_pop(std::unique_ptr<output>& out_);

private:
	settings* out_settings;
	std::atomic<std::streampos> pos;
};