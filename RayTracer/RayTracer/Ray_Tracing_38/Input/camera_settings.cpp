#include "camera_settings.h"

camera_settings::camera_settings(int mode_) :
	settings{mode_}
{
	set_input_map();
}

void camera_settings::set_mode(int mode_)
{
	if (mode_ == QUADS)
	{
		this->samples_per_pixel = 100;
		this->max_depth = 50;


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
		;		this->lookfrom = point3(278, 278, -800);
		this->lookat = point3(278, 278, 0);
		this->vfov = 40;
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
		this->background = color(0, 0, 0);
	}
	else if (mode_ == CORNELL_SMOKE)
	{
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
	}
}

void camera_settings::set_input_map()
{
	singleInputMap = {
		{"-samples_per_pixel",&samples_per_pixel},
		{"-max_depth",&max_depth},
		{"-vfov",&vfov},
		{"-fps",&fps},
		{"-num_seconds",&num_seconds},

		{"--s",&samples_per_pixel},
		{"--d",&max_depth},
		{"--v",&vfov},
		{"--f",&fps},
		{"--t",&num_seconds},
	};
}


int& camera_settings::get_samples_per_pixel()
{
	return this->samples_per_pixel;
}
int& camera_settings::get_max_depth()
{
	return this->max_depth;
}
int& camera_settings::get_vfov()
{
	return this->vfov;
}

double& camera_settings::get_defocus_angle()
{
	return defocus_angle;
}
double& camera_settings::get_focus_dist()
{
	return focus_dist;
}
point3& camera_settings::get_lookfrom()
{
	return lookfrom;
}
point3& camera_settings::get_lookat()
{
	return lookat;
}
point3& camera_settings::get_vup()
{
	return vup;
}
color& camera_settings::get_background()
{
	return background;
}
int& camera_settings::get_fps()
{
	return fps;
}
int& camera_settings::get_num_seconds()
{
	return num_seconds;
}


void camera_settings::check_validity() const
{
	settings::check_validity();
	if (samples_per_pixel <= 0)
		throw std::invalid_argument("Wrong argument for the samples_per_pixel");
	if (max_depth <= 0)
		throw std::invalid_argument("Wrong argument for the max_depth");
	if (vfov <= 0)
		throw std::invalid_argument("Wrong argument for the vfov");
	if (fps <= 0)
		throw std::invalid_argument("Wrong argument for the fps");
	if (num_seconds <= 0)
		throw std::invalid_argument("Wrong argument for the num_seconds");
}