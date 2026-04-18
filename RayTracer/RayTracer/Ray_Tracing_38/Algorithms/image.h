#pragma once
#include "camera_settings.h"
#include "../Data/color_array.h"
#include <memory>
#include <array>

class image
{
public:
	image(std::unique_ptr<camera_settings>& cam_setting_);
	virtual void returnRange(std::array<int, 2>& widthRange_, std::array<int, 2>& heightRange_) const;
	virtual void gather(std::unique_ptr<color_array>& c_array) const {};
	void returnRange(std::array<int, 2>& widthRange_, std::array<int, 2>& heightRange_) const override;
	color_array* array() { return c_array.get(); }

protected:
	// the image dimensions. 
	const int image_width, image_height;
	// the color_array object
	std::unique_ptr<color_array> c_array;

	// this rank dimensions
	int width, height;

	// ranges
	int widthMin, widthMax;
	int heightMin, heightMax;
};
