#ifndef CAMERA_H
#define CAMERA_H

#include <fstream>

#include "../Shared/rtweekend.h"

#include "../Types/color.h"
#include "../Data/color_array.h"
#include "../Geometry/hittable.h"
#include "image.h"
#include "hittable_list.h"
#include "../Materials/material.h"
#include "../Materials/material_list.h"
#include "../Types/ray.h"
#include "../Types/vec3.h"
#include "../Input/settings.h"
#include "../Input/camera_settings.h"
#include "../Algorithms/rtw_stb_image.h"




class camera {
public:

	camera() = default;
	camera(settings* cam_setting_,const image* img_);


	virtual void setup(camera_settings* cam_setting_);


	virtual void render(image* img_, const hittable& world_, const material_list& list_) const;
	virtual void render_verbose(image* img_, const hittable& world_, const material_list& list_) const;

	virtual void move_camera(point3 _lookfrom) {
		this->lookfrom = _lookfrom;
		initialize();
	}


	void print_back_ground() const
	{
		std::cout << background << std::endl;
	}

	virtual void set_range(const int& _width_min, const int& _width_max, const int& _height_min, const int& _height_max)
	{
		// I just needed that method in both the camera and camera_parallel classes
		// so that the setup in the parallel class can have generic input of camera* type
	}




protected:


	// primary parameters set by the class input;
	int samples_per_pixel = 10;
	int max_depth = 10;
	color background;
	double vfov = 90;
	point3 lookfrom = point3(0, 0, 0);
	point3 lookat = point3(0, 0, -1);
	vec3 vup = vec3(0, 1, 0);
	double focus_dist = 10;
	double defocus_angle = 0;
	// background HDRI image
	std::unique_ptr<HDRI_texture> background_image = nullptr;




	// secondary parameters set by the initialize method
	int image_width, image_height;
	double pixel_samples_scale = 1;
	point3 center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	vec3 u, v, w;
	vec3 defocus_disk_u;
	vec3 defocus_disk_v;



	// the rest of functions
	ray get_ray(int i, int j) const;
	static vec3 sample_square();
	point3 defocus_disk_sample() const;
	virtual color ray_color(const ray& r_, int depth_, const hittable& world_, const material_list& list_) const;
	color background_color(const ray& r_) const;

	void initialize();

};


#endif
