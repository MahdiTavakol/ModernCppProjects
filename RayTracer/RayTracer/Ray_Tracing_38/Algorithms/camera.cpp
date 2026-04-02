#include <fstream>

#include "../Shared/rtweekend.h"

#include "camera.h"

camera::camera(std::unique_ptr<camera_settings>& cam_setting_)
{
	setup(cam_setting_);
	initialize();
}

void camera::setup(std::unique_ptr<camera_settings>& cam_setting_)
{
	int& image_width_setting = cam_setting_->get_image_width();
	int& samples_per_pixel_setting = cam_setting_->get_samples_per_pixel();
	int& max_depth_setting = cam_setting_->get_max_depth();
	int& vfov_setting = cam_setting_->get_vfov();
	double& width_ratio_setting = cam_setting_->get_width_ratio();
	double& height_ratio_setting = cam_setting_->get_height_ratio();
	double& defocus_angle_setting = cam_setting_->get_defocus_angle();
	double& focus_dist_setting = cam_setting_->get_focus_dist();
	double& defocus_angle_setting = cam_setting_->get_defocus_angle();
	point3& lookfrom_setting = cam_setting_->get_lookfrom();
	point3& lookat_setting = cam_setting_->get_lookat();
	point3& vup_setting = cam_setting_->get_vup();
	point3& background_setting = cam_setting_->get_background();

	this->image_width = image_width_setting;
	this->aspect_ratio =
		static_cast<double>(width_ratio_setting) / static_cast<double>(height_ratio_setting);
	this->samples_per_pixel = samples_per_pixel_setting;
	this->max_depth = max_depth_setting;
	this->vfov = vfov_setting;
	this->defocus_angle = defocus_angle_setting;
	this->focus_dist = focus_dist_setting;
	this->defocus_angle = defocus_angle_setting;
	this->lookfrom = lookfrom_setting;
	this->lookat = lookat_setting;
	this->vup = vup_setting;
	this->background = background_setting;
}

void camera::initialize()
{
	image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;
	pixel_samples_scale = 1.0 / samples_per_pixel;

	center = lookfrom;

	auto theta = degrees_to_radians(vfov);
	auto h = std::tan(theta / 2.0);
	auto viewport_height = 2 * h * focus_dist;
	auto viewport_width = viewport_height * (double(image_width) / image_height);

	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);


	auto viewport_u = viewport_width * u;
	auto viewport_v = viewport_height * -v;

	pixel_delta_u = viewport_u / image_width;
	pixel_delta_v = viewport_v / image_height;

	auto viewport_upper_left = center - focus_dist * w - viewport_u / 2 - viewport_v / 2;
	pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
	defocus_disk_u = u * defocus_radius;
	defocus_disk_v = v * defocus_radius;
}


ray camera::get_ray(int i, int j) const
{
	auto offset = sample_square();
	auto pixel_sample = pixel00_loc
		+ ((i + offset.x()) * pixel_delta_u)
		+ ((j + offset.y()) * pixel_delta_v);

	auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
	auto ray_direction = pixel_sample - ray_origin;
	auto ray_time = random_double();

	return ray(ray_origin, ray_direction, ray_time);
}

vec3 camera::sample_square() const
{
	return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

point3 camera::defocus_disk_sample() const
{
	auto p = random_in_unit_disk();
	return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

color camera::ray_color(const ray& r, int depth, const hittable& world) const
{
	if (depth <= 0)
		return color(0, 0, 0);

	hit_record rec;

	if (!world.hit(r, interval(0.001, infinity), rec))
		return background;


	ray scattered;
	color attenuation;
	color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

	if (!rec.mat->scatter(r, rec, attenuation, scattered))
		return color_from_emission;


	color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);

	return color_from_emission + color_from_scatter;
}

