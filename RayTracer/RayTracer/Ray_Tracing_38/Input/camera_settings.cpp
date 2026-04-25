#include "camera_settings.h"

camera_settings::camera_settings(int mode_) :
	settings{mode_}
{
	set_mode(mode_);
	set_input_map();
}

void camera_settings::set_mode(int mode_)
{
	switch (mode_)
	{
	case QUADS:
		this->samples_per_pixel = 100;
		this->max_depth = 50;
		this->vfov = 80;
		this->lookat = point3(0, 0, 0);
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
		break;
	case TWO_LIGHTS:
	case SIMPLE_LIGHT:
		this->lookat = point3(0, 1, 0);
		this->defocus_angle = 0;
		this->vfov = 20;
		this->background = color(0, 0, 0);
		break;
	case CORNELL_BOX:
	case TWO_BOXES:
	case TWO_BOXES_ROTATED:
		this->lookat = point3(278, 278, 0);
		this->vfov = 40;
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
		this->background = color(0, 0, 0);
		break;
	case CORNELL_SMOKE:
		this->background = color(0, 0, 0);
		this->vfov = 40;
		this->lookat = point3(278, 278, 0);
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
		break;
	case RANDOM_SPHERES_ANIMATED:
		this->background = color(0.7, 0.8, 1.00);
		// The rest of the thing
		break;
	case SIMPLE_2D_PARALEL_TEST:
		this->background = color(0.7, 0.8, 1.00);
		this->lookat = point3(0, 0, 0);
		this->vup = vec3(0, 1, 0);
		this->vfov = 20;
		this->defocus_angle = 0;
		break;
	default:
		throw std::invalid_argument("Wrong mode!");
	}
}

void camera_settings::set_input_map()
{
	singleInputMap = {
		{"-samples_per_pixel",&samples_per_pixel},
		{"-max_depth",&max_depth},
		{"-vfov",&vfov},

		{"--s",&samples_per_pixel},
		{"--d",&max_depth},
		{"--v",&vfov}
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


void camera_settings::check_validity() const
{
	settings::check_validity();
	if (samples_per_pixel <= 0)
		throw std::invalid_argument("Wrong argument for the samples_per_pixel");
	if (max_depth <= 0)
		throw std::invalid_argument("Wrong argument for the max_depth");
	if (vfov <= 0)
		throw std::invalid_argument("Wrong argument for the vfov");
}

void camera_settings::log_class_name(std::iostream& stream_) const
{
	stream_ << "Camera class options:" << std::endl << std::endl;
}