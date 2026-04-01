#include "camera_settings.h"

camera_settings::camera_settings(int mode_) :
	mode{ mode_ }
{
	if (mode_ == QUADS)
	{
		this->image_width = 1080;
		this->samples_per_pixel = 100;
		this->max_depth = 50;

		this->width_ratio = 1.0;
		this->height_ratio = 1.0;

		this->vfov = 80;
		this->lookfrom = point3(0, 0, 9);
		this->lookat = point3(0, 0, 0);
		this->vup = vec3(0, 1, 0);

		this->defocus_angle = 0;
	}
	else if (mode_ == TWO_LIGHTS || mode_ == SIMPLE_LIGHT)
	{
		this->lookfrom = point3(26, 3, 6);
		this->lookat = point3(0, 1, 0);
		this->defocus_angle = 0;
		this->vfov = 20;
		this->background = color(0, 0, 0);
	}
	else if (mode_ == CORNELL_BOX || mode_ == TWO_BOXES || mode_ == TWO_BOXES_ROTATED)
	{
		this->lookfrom = point3(278, 278, -800);
		this->lookat = point3(278, 278, 0);
		this->vfov = 40;
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
		this->background = color(0, 0, 0);
		this->width_ratio = 1.0;
		this->height_ratio = 1.0;
	}
	else if (mode_ == CORNELL_SMOKE)
	{
		this->width_ratio = 1.0;
		this->height_ratio = 1.0;
		this->background = color(0, 0, 0);

		this->vfov = 40;
		this->lookfrom = point3(278, 278, -800);
		this->lookat = point3(278, 278, 0);
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
	}
	else if (mode_ == RANDOM_SPHERES_ANIMATED)
	{
		this->fps = 60;
		this->num_seconds = 10;
		this->background = color(0.7, 0.8, 1.00);
		// The rest of the thing
	}
	else if (mode_ == SIMPLE_2D_PARALEL_TEST)
	{
		this->background = color(0.7, 0.8, 1.00);
		this->lookfrom = point3(100, 100, 500);
		this->lookat = point3(0, 0, 0);
		this->vup = vec3(0, 1, 0);
		this->vfov = 20;
		this->defocus_angle = 0;
		this->image_width = 1080;
	}
}