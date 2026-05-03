#include <fstream>

#include "../Shared/rtweekend.h"

#include "camera.h"

camera::camera(std::unique_ptr<camera_settings>& cam_setting_, std::unique_ptr<image>&& img_):
	img{std::move(img_)}
{
	setup(cam_setting_.get());
	initialize();
}

camera::camera(settings* cam_setting_, std::unique_ptr<image>&& img_) :
	img{ std::move(img_) }
{
	// checking the setting type
	camera_settings* sett = dynamic_cast<camera_settings*>(cam_setting_);
	if (!sett)
		throw std::invalid_argument("Wrong settings object");

	setup(sett);
	initialize();
}

void camera::setup(camera_settings* cam_setting_)
{
	int& samples_per_pixel_setting = cam_setting_->get_samples_per_pixel();
	int& max_depth_setting = cam_setting_->get_max_depth();
	int& vfov_setting = cam_setting_->get_vfov();
	double& focus_dist_setting = cam_setting_->get_focus_dist();
	double& defocus_angle_setting = cam_setting_->get_defocus_angle();
	point3& lookat_setting = cam_setting_->get_lookat();
	point3& vup_setting = cam_setting_->get_vup();
	point3& background_setting = cam_setting_->get_background();


	this->samples_per_pixel = samples_per_pixel_setting;
	this->max_depth = max_depth_setting;
	this->vfov = vfov_setting;
	this->defocus_angle = defocus_angle_setting;
	this->focus_dist = focus_dist_setting;
	this->defocus_angle = defocus_angle_setting;
	this->lookat = lookat_setting;
	this->vup = vup_setting;
	this->background = background_setting;
}

void camera::initialize()
{
	pixel_samples_scale = 1.0 / samples_per_pixel;

	int image_width, image_height;
	img->returnSize(image_width, image_height);

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


void camera::render(const hittable& world_)
{
	// getting the color_data array from the image object
	color_data** c_data = img->returnColorData();
	// getting the ranges from the image object
	std::array<int, 2> widthRange, heightRange;
	img->returnRange(widthRange, heightRange);
	int height_min = heightRange[0];
	int height_max = heightRange[1];
	int width_min = widthRange[0];
	int width_max = widthRange[1];


	for (int j = height_min; j < height_max; j++)
	{
		for (int i = width_min; i < width_max; i++)
		{
			color pixel_color(0, 0, 0);
			for (int sample = 0; sample < samples_per_pixel; sample++)
			{
				ray r = get_ray(i, j);
				pixel_color += ray_color(r, max_depth, world_);
			}
			pixel_color = pixel_samples_scale * pixel_color;
			c_data[i - width_min][j - height_min].r = pixel_color.x();
			c_data[i - width_min][j - height_min].g = pixel_color.y();
			c_data[i - width_min][j - height_min].b = pixel_color.z();
		}
	}
}

void camera::render_verbose(const hittable& world_)
{
	// getting the color_data array from the image object
	color_data** c_data = img->returnColorData();
	// getting the ranges from the image object
	std::array<int, 2> widthRange, heightRange;
	img->returnRange(widthRange, heightRange);
	int height_min = heightRange[0];
	int height_max = heightRange[1];
	int width_min = widthRange[0];
	int width_max = widthRange[1];

	int image_height = height_max - height_min;
	int image_width = width_max - width_min;

	for (int j = height_min; j < height_max; j++)
	{
		std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;
		for (int i = width_min; i < width_max; i++)
		{
			color pixel_color(0, 0, 0);
			samples_per_pixel = 1;
			for (int sample = 0; sample < samples_per_pixel; sample++)
			{
				ray r = get_ray(i, j);
				pixel_color += ray_color(r, max_depth, world_);
			}
			pixel_color = pixel_samples_scale * pixel_color;
			c_data[i][j].r = pixel_color.x();
			c_data[i][j].g = pixel_color.y();
			c_data[i][j].b = pixel_color.z();
		}
	}

	std::clog << "\rDone                          ";
}

void camera::render_async(const hittable& world_, image* img_)
{
	// getting the color_array pointer from the image object
	color_array* c_array = img->array();
	// getting the ranges from the image object
	std::array<int, 2> widthRange, heightRange;
	img->returnRange(widthRange, heightRange);
	int height_min = heightRange[0];
	int height_max = heightRange[1];
	int width_min = widthRange[0];
	int width_max = widthRange[1];


	for (int j = height_min; j < height_max; j++)
	{
		for (int i = width_min; i < width_max; i++)
		{
			color pixel_color(0, 0, 0);
			for (int sample = 0; sample < samples_per_pixel; sample++)
			{
				ray r = get_ray(i, j);
				pixel_color += ray_color(r, max_depth, world_);
			}
			pixel_color = pixel_samples_scale * pixel_color;
			color_data c_data{ pixel_color.x(),pixel_color.y(),pixel_color.z() };
			c_array->set(i - width_min, j - height_min, c_data);
		}
	}
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


void camera::reset_image(std::unique_ptr<image>&& img_)
{
	img = std::move(img_);
}
