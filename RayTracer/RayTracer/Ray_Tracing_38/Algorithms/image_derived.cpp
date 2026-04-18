#include "image_derived.h"


image_derived::image_derived(std::unique_ptr<camera_settings>& cam_setting_):
	image{ cam_setting_ }
{
	c_array = std::make_unique<color_array>(width, height);
}

