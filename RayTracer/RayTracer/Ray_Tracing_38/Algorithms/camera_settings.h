#pragma once

#include "../Types/vec3.h"
#include "../Types/color.h"
#include "../Shared/rtweekend.h"


class camera_settings
{
public:
	camera_settings()
	{}
	camera_settings(int mode_);
	int& get_image_width();
	int& get_samples_per_pixel();
	int& get_max_depth();
	int& get_vfov();
	double& get_width_ratio();
	double& get_height_ratio();
	double& get_defocus_angle();
	double& get_focus_dist();
	point3& get_lookfrom();
	point3& get_lookat();
	point3& get_vup();
	color& get_background();
	int& get_fps();
	int& get_num_seconds();

protected:
	int mode = 0;

	int image_width = 1080;
	int samples_per_pixel = 150;
	int max_depth = 50;
	int vfov = 45;
	double width_ratio = 16.0;
	double height_ratio = 9.0;
	double defocus_angle = 0.6;
	double focus_dist = 10.0;
		
	point3 lookfrom = point3(13, 2, 3);
	point3 lookat = point3(0, 0, 0);
	point3 vup = point3(0, 1, 0);

	color background = color(0.7, 0.8, 1.00);

	int fps = 1;
	int num_seconds = 1;

};