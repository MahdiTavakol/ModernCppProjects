#pragma once

#include "image.h"

class image_derived : public image
{
public:
	image_derived(std::unique_ptr<camera_settings>& cam_setting_);
};