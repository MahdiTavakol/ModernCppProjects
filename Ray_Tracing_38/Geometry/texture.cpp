#include "texture.h"

color solid_color::value(double _u, double _v, const point3& p) const
{
	return albedo;
}

checker_texture::checker_texture(double _scale, std::unique_ptr<texture> _even, std::unique_ptr<texture> _odd)
	: inv_scale{ 1.0 / _scale }, even{ std::move(_even) }, odd{ std::move(_odd) } {
}

checker_texture::checker_texture(double _scale, const color& _c1, const color& _c2)
	: checker_texture(_scale, std::make_unique<solid_color>(_c1), std::make_unique<solid_color>(_c2)) {
}

color checker_texture::value(double _u, double _v, const point3& _p) const
{
	auto xInteger = int(std::floor(inv_scale * _p.x()));
	auto yInteger = int(std::floor(inv_scale * _p.y()));
	auto zInteger = int(std::floor(inv_scale * _p.z()));

	bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

	return isEven ? even->value(_u, _v, _p) : odd->value(_u, _v, _p);
}

image_texture::image_texture(const char* _filename) : image(_filename) {}
image_texture::image_texture(std::string filename_) : image(filename_.c_str()) {}

color image_texture::value(double u, double v, const point3& p) const
{
	if (image.height() <= 0) return color(0, 1, 1);

	u = interval(0, 1).clamp(u);
	v = 1.0 - interval(0, 1).clamp(v);

	auto i = int(u * image.width());
	auto j = int(v * image.height());
	auto pixel = image.pixel_data(i, j);

	auto color_scale = 1.0 / 255.0;
	return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}

HDRI_texture::HDRI_texture(std::string _filename) :image(_filename.c_str()) {}

color HDRI_texture::value(double u, double v, const point3& p) const
{
	if (image.height() <= 0) return color(0, 1, 1);

	u = interval(0, 1).clamp(u);
	v = interval(0, 1).clamp(v);

	auto i = int(u * image.width());
	auto j = int(v * image.height());
	const float* pixel = image.pixel_raw(i, j);

	return color(pixel[0], pixel[1], pixel[2]);
}

noise_texture::noise_texture(double _scale) : scale(_scale) {}

color noise_texture::value(double _u, double _b, const point3& _p) const
{
	return color(0.5, 0.5, 0.5) * (1 + std::sin(scale * _p.z() + 10 * noise.turb(_p, 7)));
	//return color(1, 1, 1) * noise.turb(p, 7);
	//return color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * _p));
}