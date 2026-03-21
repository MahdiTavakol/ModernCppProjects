#include "camera_derived.h"

void camera_derived::render(const hittable& world) {

	for (int j = 0; j < image_height; j++)
	{
		std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;
		for (int i = 0; i < image_width; i++)
		{
			color pixel_color(0, 0, 0);
			samples_per_pixel = 1;
			for (int sample = 0; sample < samples_per_pixel; sample++)
			{
				ray r = get_ray(i, j);
				pixel_color += ray_color(r, max_depth, world);
			}
			pixel_color = pixel_samples_scale * pixel_color;
			color_data** c_data = c_array.return_array();
			c_data[i][j].r = pixel_color.x();
			c_data[i][j].g = pixel_color.y();
			c_data[i][j].b = pixel_color.z();
		}
	}

	std::clog << "\rDone                          ";
}