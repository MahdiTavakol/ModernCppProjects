#pragma once

#include "image.h"
#include "queue.h"


class image_async : public image
{
public:
	image_async(settings* img_setting_,
		communicator* para_,
		std::array<int,2> domain_size_ = {16,9});


	image_async(const image_async& rhs_) = delete;
	image_async& operator=(const image_async& rhs_) = delete;


	void push(std::unique_ptr<image> img_);
	bool try_pop(std::unique_ptr<image>& img_);

private:
	queue_4<image> image_queue;
	const std::array<int, 2> domain_size;
};
