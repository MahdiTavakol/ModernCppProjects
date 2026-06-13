#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Types/color.h"
#include "../Algorithms/rtw_stb_image.h"
#include "perlin.h"

class texture
{
public:
	virtual ~texture() = default;

	virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture
{
public:
	solid_color(const color& _albedo) : albedo(_albedo) {}

	solid_color(double _red, double _green,double _blue): 
		solid_color(color(_red,_green,_blue)) {}

	color value(double _u, double _v, const point3& p) const override;

private:
	color albedo;
};

class checker_texture : public texture
{
public:
	checker_texture(double _scale, std::unique_ptr<texture> _even, std::unique_ptr<texture> _odd);

	checker_texture(double _scale, const color& _c1, const color& _c2);

	color value(double _u, double _v, const point3& _p) const override;

private:
	double inv_scale;
	std::unique_ptr<texture> even;
	std::unique_ptr<texture> odd;
};

class image_texture : public texture
{
public:
	image_texture(const char* _filename);
	image_texture(std::string filename_);

	color value(double u, double v, const point3& p) const override;

private:
	rtw_image image;
};

class HDRI_texture : public texture
{
public:
	HDRI_texture(std::string _filename);

	color value(double u, double v, const point3& p) const override;
private:
	rtw_image image;
};

class noise_texture : public texture
{
public:
	noise_texture(double _scale);
	color value(double _u, double _b, const point3& _p) const override;
private:
	perlin noise;
	double scale;
};

#endif