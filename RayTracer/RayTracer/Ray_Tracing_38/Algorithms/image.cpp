#include "image.h"

image::image(std::unique_ptr<camera_settings>& cam_setting_,
	std::unique_ptr<parallel>& _para) :
	para{_para.get()},
	image_width{ cam_setting_->get_image_width()},
	image_height{cam_setting_->get_image_height()}
{
	width = image_width;
	height = image_height;

	widthMin = 0;
	heightMin = 0;
	widthMax = image_width;
	heightMax = image_height;
}

void image::returnRange(std::array<int, 2>& widthRange_, std::array<int, 2>& heightRange_) const
{
	widthRange_[0] = widthMin;
	widthRange_[1] = widthMax;
	heightRange_[0] = heightMin;
	heightRange_[1] = heightMax;
}

void image::returnSize(int& width_, int& height_) const
{
	c_array->return_size(width_, height_);
}