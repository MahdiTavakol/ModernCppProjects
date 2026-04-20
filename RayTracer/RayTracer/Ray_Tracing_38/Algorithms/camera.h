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
#include "../Types/ray.h"
#include "../Types/vec3.h"
#include "camera_settings.h"




class camera {
public:

	camera(std::unique_ptr<camera_settings>& cam_setting_, std::unique_ptr<image>&& img_);
	camera() = default;


	virtual void setup(std::unique_ptr<camera_settings>& cam_setting_);

	virtual void render(const hittable& world);
	virtual void render_verbose(const hittable& world);

	virtual void move_camera(point3 _lookfrom) {
		this->lookfrom = _lookfrom;
	}

	void return_image_size(int& _image_width, int& _image_height) const
	{
		_image_width = this->image_width;
		_image_height = this->image_height;
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

	std::unique_ptr<image> return_image()
	{
		if (img == nullptr)
			throw std::runtime_error("The image has already been returned!");
		return std::move(img);
	}

	color_array* return_color_array_ptr()
	{
		return img->array();
	}



protected:


	// primary parameters set by the class input;
	double aspect_ratio = 1.0;
	int image_width = 100;
	int image_height = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;
	color background;
	double vfov = 90;
	point3 lookfrom = point3(0, 0, 0);
	point3 lookat = point3(0, 0, -1);
	vec3 vup = vec3(0, 1, 0);
	double defocus_angle = 0;
	double focus_dist = 10;




	// secondary parameters set by the initialize method
	double pixel_samples_scale = 1;
	point3 center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	vec3 u, v, w;
	vec3 defocus_disk_u;
	vec3 defocus_disk_v;

	// the rendered image
	std::unique_ptr<image> img;


	// the rest of functions
	ray get_ray(int i, int j) const;
	vec3 sample_square() const;
	point3 defocus_disk_sample() const;
	virtual color ray_color(const ray& r, int depth, const hittable& world) const;
	void initialize();
};


#endif
