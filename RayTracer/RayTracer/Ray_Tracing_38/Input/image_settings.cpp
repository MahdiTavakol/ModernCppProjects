#include "image_settings.h"
#include "../Shared/rtweekend.h"

image_settings::image_settings(int mode_):
	settings{mode_}
{
	set_input_map();
}

void image_settings::set_input_map()
{
	singleInputMap = {
		{"-width",&image_width},
		{"--w",&image_width}
	};

	doubleInputMap = {
		{"-aspect_ratio",std::pair{&width_ratio,&height_ratio}},
		{"--a",std::pair{&width_ratio,&height_ratio}},
	};
}

void image_settings::set_mode(int mode_)
{
	if (mode_ == QUADS)
	{
		this->image_width = 1080;
	}
	else if (mode_ == TWO_LIGHTS || mode_ == SIMPLE_LIGHT)
	{
	}
	else if (mode_ == CORNELL_BOX || mode_ == TWO_BOXES || mode_ == TWO_BOXES_ROTATED)
	{
		this->width_ratio = 1.0;
		this->height_ratio = 1.0;
	}
	else if (mode_ == CORNELL_SMOKE)
	{
		this->width_ratio = 1.0;
		this->height_ratio = 1.0;
	}
	else if (mode_ == RANDOM_SPHERES_ANIMATED)
	{
	}
	else if (mode_ == SIMPLE_2D_PARALEL_TEST)
	{
		this->image_width = 1080;
	}
}

int& image_settings::get_image_width()
{
	return this->image_width;
}
int image_settings::get_image_height()
{
	return this->image_width * height_ratio / width_ratio;
}

double& image_settings::get_width_ratio()
{
	return width_ratio;
}
double& image_settings::get_height_ratio()
{
	return height_ratio;
}


void image_settings::check_validity() const
{
	settings::check_validity();
	if (image_width <= 0)
		throw std::invalid_argument("Invalid argument for the image_width");
	if (width_ratio <= 0 || height_ratio <= 0)
		throw std::invalid_argument("Invalid argument for the height_ratio or the width_ratio");
}