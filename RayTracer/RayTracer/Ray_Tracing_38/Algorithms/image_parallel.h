#pragma once

#include "image.h"
#include "parallel.h"

class image_parallel : public image
{
public:
	image_parallel(
		std::unique_ptr<camera_settings>& _cam_setting,
		std::unique_ptr<parallel>& _para
	);
	void gather(std::unique_ptr<color_array>& c_array_) const override;


protected:
	parallel* para;
	int width_per_rank, height_per_rank;

};
