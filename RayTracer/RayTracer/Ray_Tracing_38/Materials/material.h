#ifndef MATERIAL_H
#define MATERIAL_H


#include "../Shared/rtweekend.h"

#include "../Types/ray.h"
#include "../Types/color.h"
#include "../Geometry/texture.h"
#include "../Algorithms/hit_record.h"
#include <array>
#include <vector>

#include "../Input/tiny_gltf_v3.h"

struct scatter_record
{
	ray scattered_ray;
	color attenuation;
	double weight;
	bool scattered = false;
};


class material
{
public:
	virtual ~material() = default;


	virtual color emitted(double _u, double _v, const point3& _p) const
	{
		return color(0, 0, 0);
	}

	virtual void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const
	{
	}

	virtual bool is_equal( const material& _second) const = 0;

	virtual bool compare(material* _rhs, const double tol_) const
	{
		std::cout << "The compare function has not been implemented yet " << std::endl;
		return true;
	}

	bool operator==(const material& _second) {
		return (typeid(*this) == typeid(_second)) && (is_equal(_second));
	}

};

// general material class for mtl format material
class general : public material
{
public:
	general(const color& _albedo, const double& _shininess,
		 const double& _Tr, const color& _Tf, const color _Ks, const double _Ni) :
		albedo(_albedo), shininess(_shininess), Tr(_Tr), Tf(_Tf), Ks(_Ks), Ni(_Ni)
	{
	}


	void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const override
	{
		double transparency = std::clamp(Tr, 0.0, 1.0);
		double opacity = 1.0 - transparency;
		double specular_strength = std::clamp(std::max(Ks.x(), std::max(Ks.y(), Ks.z())), 0.0, 1.0);
			
		double diffuse_weight = opacity * (1.0 - specular_strength);
		double specular_weight = opacity * specular_strength;
		double transmit_weight = transparency;

		// setting the srec to all zeros
		srec_[0] = srec_[1] = srec_[2] = { ray(rec.p, vec3(0, 0, 0), r_in.time()), color(0, 0, 0), 0.0, false };

		if (Tr > 0.7)
		{
			transmit_weight = 1.0;
			transmit_scatter(r_in, rec, srec_[2], transmit_weight);
		}
		else if (specular_strength > 0.5)
		{
			specular_weight = 1.0;
			specular_scatter(r_in, rec, srec_[1], specular_weight);
		}
		else
		{
			if (diffuse_weight > 0.05)
			{
				diffuse_scatter(r_in, rec, srec_[0], diffuse_weight);
			}


			if (specular_weight > 0.05)
			{
				specular_scatter(r_in, rec, srec_[1], specular_weight);
			}

			if (transmit_weight > 0.05)
			{
				transmit_scatter(r_in, rec, srec_[2], transmit_weight);
			}
		}

		return;
	}

	void diffuse_scatter(const ray& r_in, const hit_record& rec, scatter_record& srec_, double& weight_) const
	{
		vec3 diffuse_dir = rec.normal + random_unit_vector();
		if (diffuse_dir.near_zero())
		{
			diffuse_dir = rec.normal;
		}
		srec_ = scatter_record{ ray(rec.p, diffuse_dir, r_in.time()), albedo, weight_, true };
	}

	void specular_scatter(const ray& r_in, const hit_record& rec, scatter_record& srec_, double& weight_) const
	{
		vec3 unit_dir = unit_vector(r_in.direction());
		double roughness = std::sqrt(2.0 / (shininess + 2.0));
		roughness = std::clamp(roughness, 0.0, 1.0);

		vec3 reflected = reflect(unit_dir, rec.normal);
		reflected = unit_vector(reflected);
		reflected += roughness * random_unit_vector();
		srec_ = scatter_record{ ray(rec.p, unit_vector(reflected), r_in.time()), Ks, weight_, true};
	}

	void transmit_scatter(const ray& r_in, const hit_record& rec, scatter_record& srec_, double& weight_) const
	{
		vec3 unit_dir = unit_vector(r_in.direction());
		double refraction_ratio = rec.front_face ? (1.0 / Ni) : Ni;

		vec3 direction;

		double cos_theta = std::min(dot(-unit_dir, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;


		if (cannot_refract)
		{
			direction = reflect(unit_dir, rec.normal);
		}
		else
		{
			direction = refract(unit_dir, rec.normal, refraction_ratio);
		}
		srec_ = { ray(rec.p, direction, r_in.time()), Tf, weight_, true };
	}

	bool is_equal(const material& _second) const override {
		const general* o = dynamic_cast<const general*>(&_second);
		return o && (albedo == o->albedo) && (shininess == o->shininess) &&
			(Tr == o->Tr) && (Tf == o->Tf) && (Ks == o->Ks);
	}

	bool compare(material* rhs_, const double tol_) const override
	{
		general* o = dynamic_cast<general*>(rhs_);
		if (!o) {
			std::cout << "Material type mismatch" << std::endl;
			return true;
		}
		if (std::abs(shininess - o->shininess) >= tol_)
			return true;
		if (std::abs(Tr - o->Tr) >= tol_)
			return true;
		color deltaAlb = albedo - o->albedo;
		color deltaKa = Ka - o->Ka;
		color deltaKs = Ks - o->Ks;
		color deltaTf = Tf - o->Tf;
		double dAlbLength = deltaAlb.length();
		double dKaLength = deltaKa.length();
		double dKsLength = deltaKs.length();
		double dTfLength = deltaTf.length();
		if (dAlbLength >= tol_ ||
			dKaLength  >= tol_ ||
			dKsLength  >= tol_ ||
			dTfLength  >= tol_)
			return true;
		return false;
	}



private:
	color albedo;
	double shininess;
	double Tr;
	color Ka, Ks, Tf;
	double Ni;
};

// PBR material for gltf 
class PBR : public material
{
public:
	PBR(const tg3_material prop_):
		prop{prop_}
	{
		// deep copies is not possible since member functions are const pointer!

		// returning warnings for ignored extras
		tg3_extras_ext mat_ext = prop.ext;
		if (mat_ext.extras != NULL)
			std::cout << "warning: ignoring the extras for the material" << std::endl;
		tg3_extras_ext pbr_ext = prop.pbr_metallic_roughness.ext;
		if (pbr_ext.extras != NULL)
			std::cout << "Warning: ignoring extras for the pbr_metallic_roughness" << std::endl;
		tg3_extras_ext base_color_ext = prop.pbr_metallic_roughness.base_color_texture.ext;
		if (base_color_ext.extras != NULL)
			std::cout << "Warning: ignoring extras for the pbr_metallic_roughness base_color_texture" << std::endl;
		tg3_extras_ext met_rough_ext = prop.pbr_metallic_roughness.metallic_roughness_texture.ext;
		if (met_rough_ext.extras != NULL)
			std::cout << "Warning: ignoring extras for the pbr_metallic_roughness metall_roughness" << std::endl;
		tg3_extras_ext norm_ext = prop.normal_texture.ext;
		if (norm_ext.extras != NULL)
			std::cout << "Warning: ignoring the extras for the normal_texture" << std::endl;
		tg3_extras_ext occl_ext = prop.occlusion_texture.ext;
		if (occl_ext.extras != NULL)
			std::cout << "Warning: ignoring the extras for the occlusion_texture" << std::endl;
		tg3_extras_ext emmi_ext = prop.emissive_texture.ext;
		if (emmi_ext.extras != NULL)
			std::cout << "Warning: ignoring the extras for the emission_texture" << std::endl;

	}

	void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const override
	{
		srec_[0] = { ray(rec.p, vec3(0, 0, 0), r_in.time()), color(0, 0, 0), 0.0, false };

		// getting u, v coordinates
		double u = rec.u, v = rec.v;
		double u1 = rec.u1, v1 = rec.v1;
		// getting the base color
		const double* base_col = prop.pbr_metallic_roughness.base_color_factor;
		const tg3_texture_info* texture = &prop.pbr_metallic_roughness.base_color_texture;
		int32_t tex_coord = texture->tex_coord;

		vec4 vc = tg3_texture_info_to_color(texture, u, v,u1, v1);
		vec3 albedo = vec3{ vc[0]*base_col[0],vc[1]*base_col[1],vc[2]*base_col[2]};

		// reflected part
		// reading the roughness texture
		texture = &prop.pbr_metallic_roughness.metallic_roughness_texture;
		vc = tg3_texture_info_to_color(texture, u, v, u1, v1);
		double metallic_weight = vc[2]; // the blue channel
		double roughness = vc[1]; // the green channel
		// metallic 
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		double metallic_factor = prop.pbr_metallic_roughness.metallic_factor;
		metallic_weight *= metallic_factor;
		metallic_weight = std::clamp(metallic_weight, 0.0,1.0);
		double diffuse_weight = 1.0 - metallic_weight;

		// roughness 

		vec3 random_vector = random_unit_vector();
		double roughness_factor = prop.pbr_metallic_roughness.roughness_factor;
		vec3 roughReflected = roughness *
			vec3{ random_vector[0],
				  random_vector[1],
			      random_vector[2] };

		reflected += roughness_factor * roughReflected;
		reflected = unit_vector(reflected);

		srec_[1] = { ray(rec.p, reflected, r_in.time()), albedo, metallic_weight, true };


		// diffusive part
		vec3 diffuse = random_unit_vector() + rec.normal;
		diffuse = unit_vector(diffuse);
		srec_[2] = { ray(rec.p,diffuse,r_in.time()),albedo,diffuse_weight,true };
	}

	color emitted(double _u, double _v, const point3& _p)  const override
	{
		const double* emissive_factor = prop.emissive_factor;
		const tg3_texture_info* texture = &prop.emissive_texture;
		vec4 vc = tg3_texture_info_to_color(texture, _u, _v);

		vc[0] *= emissive_factor[0];
		vc[1] *= emissive_factor[1];
		vc[2] *= emissive_factor[2];

		color clr{ vc[0],vc[1],vc[2] };
		return clr;
	}

	bool is_equal(const material& _second) const override
	{
		return false;
	}

	static void add_images(const tg3_image* imagePtr_, const int& image_count_)
	{
		for (int i = 0; i < image_count_; i++)
		{
			images.push_back(imagePtr_[i]);
		}
	}

	vec4 tg3_texture_info_to_color(
		const tg3_texture_info* texture_,
		const double& u_,
		const double& v_
	) const
	{
		int32_t index = texture_->index;
		int32_t coord = texture_->tex_coord;
		if (coord != 0)
		{
			std::cout << "Warning: currently coords higher than 0 is not supported!";
		}
		if (index >= images.size())
		{
			std::cout << "Warning: currently coords higher than 0 is not supported!";
		}

		vec4 vc{ 1.0,1.0,1.0,1.0 };
		if (index != -1)
		{
			tg3_image* img = &images[index];
			int x = static_cast<int>(u_ * (img->width - 1));
			int y = static_cast<int>((1.0 - v_) * (img->height - 1));
			vc = tg3_image_to_color(img, x, y);
		}
		return vc;
	}

	vec4 tg3_texture_info_to_color(
		const tg3_texture_info* texture_,
		const double& u_, 
		const double& v_,
		const double& u1_, 
		const double& v1_) const
	{
		int32_t index = texture_->index;
		int32_t coord = texture_->tex_coord;

		if (coord != 0)
		{
			std::cout << "Warning: currently coords higher than 0 is not supported!";
		}
		if (index >= images.size())
		{
			throw std::runtime_error("Out of range access for images!");
		}

		tg3_image* img = &images[index];
		vec4 vc{ 1.0,1.0,1.0,1.0 };
		if (index != -1)
		{
			int x, y;
			if (coord == 0) {
				x = static_cast<int>(u_ * (img->width - 1));
				y = static_cast<int>((1.0 - v_) * (img->height - 1));
			}
			else if (coord == 1)
			{
				x = static_cast<int>(u1_ * (img->width - 1));
				y = static_cast<int>((1.0 - v1_) * (img->height - 1));
			}
			else
			{
				std::cout << "Warning: currently coords higher than 0 is not supported!";
				x = static_cast<int>(u_ * (img->width - 1));
				y = static_cast<int>((1.0 - v_) * (img->height - 1));
			}
			vc = tg3_image_to_color(img, x, y);
		}
		return vc;
	}

	static vec4 tg3_image_to_color(
		const tg3_image* image_, 
		const int& x_,
		const int& y_)
	{
		tg3_span_u8 image_raw = image_->image;
		int dataPerPixel = image_->component;
		int dataPerRow = image_->component * image_->width;
		int ind1 = y_ * dataPerRow + x_ * dataPerPixel;
		int ind2 = ind1 + 1;
		int ind3 = ind1 + 2;
		int ind4 = ind1 + 3;
		if (ind4 >= image_raw.count)
			throw std::runtime_error("Out of range access!");

		vec4 vc{ 
			image_raw.data[ind1]/255.0,
			image_raw.data[ind2]/255.0,
			image_raw.data[ind3]/255.0,
			image_raw.data[ind4]/255.0};

		return vc;
	}


private:
	tg3_material prop;
	inline static std::vector<tg3_image> images;
};


class lambertian : public material
{
public:
	lambertian(const color& _albedo) : tex(std::make_unique<solid_color>(_albedo)) {}
	lambertian(std::unique_ptr<texture> _tex) :
		tex{ std::move(_tex) }
	{}


	void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;
		srec_[0] = { ray(rec.p, scatter_direction, r_in.time()), tex->value(rec.u, rec.v, rec.p), 1.0, true };
		srec_[1] = srec_[2] = { ray(rec.p, vec3(0, 0, 0), r_in.time()), color(0, 0, 0), 0.0, false };
	}

	bool is_equal(const material& _second) const override {
		const lambertian* o = dynamic_cast<const lambertian*>(&_second);
		return o && false;
		//return o && (*tex == *(o->tex)); 
	}

private:
	std::unique_ptr<texture> tex;
};

class metal : public material
{
public:
	metal() : albedo(vec3(0, 0, 0)), fuzz(1.0) {}

	metal(const color& _albedo, double _fuzz) : albedo(_albedo), fuzz(_fuzz) {}



	void scatter(const ray& r_in, const hit_record& rec,  std::array<scatter_record, 3>& srec_) const override
	{
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + fuzz * random_unit_vector();
		srec_[1] = { ray(rec.p, reflected, r_in.time()), albedo, 1.0, true };
		srec_[0] = srec_[2] = { ray(rec.p, vec3(0, 0, 0), r_in.time()), color(0, 0, 0), 0.0, false };
	}

	void return_params(color& _albedo, double& _fuzz)
	{
		_fuzz = fuzz;
		_albedo = albedo;
	}

	bool is_equal(const material& _second) const override {
		const metal* o = dynamic_cast<const metal*>(&_second);
		return o && (albedo == o->albedo) && (fuzz == o->fuzz);
	}

private:
	color albedo;
	double fuzz;
};

class dielectric : public material
{
public:
	dielectric(double _refraction_index, color attenuation_ = color(1.0, 1.0, 1.0)) : 
		refraction_index(_refraction_index),
		attenuation{ attenuation_ }
	{}


	void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const override
	{
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, ri) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, ri);

		srec_[2] = { ray(rec.p + 1e-4 * direction, direction, r_in.time()), attenuation, 1.0, true };
		srec_[0] = srec_[1] = { ray(rec.p, vec3(0, 0, 0), r_in.time()), color(0, 0, 0), 0.0, false };
	}

	void return_params(double& _ref)
	{
		_ref = refraction_index;
	}

	bool is_equal(const material& _second) const override {
		const dielectric* o = dynamic_cast<const dielectric*>(&_second);
		return o && (refraction_index == o->refraction_index);
	}

private:
	color attenuation;
	double refraction_index;

	static double reflectance(double cosine, double refraction_index)
	{
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};


class diffuse_light : public material {
public:
	diffuse_light(std::unique_ptr<texture>& _tex) : tex{ std::move(_tex) } {}
	diffuse_light(const color& _emit) : tex{ std::make_unique<solid_color>(_emit) } {}


	color emitted(double _u, double _v, const point3& _p)  const override {
		return tex->value(_u, _v, _p);
	}

	bool is_equal(const material& _second) const override {
		const diffuse_light* o = dynamic_cast<const diffuse_light*>(&_second);
		return o && (tex == o->tex);
	}

private:
	std::unique_ptr<texture> tex;
};

class isotropic : public material
{
public:
	isotropic(const color& _albedo) : tex(std::make_unique<solid_color>(_albedo)) {}
	isotropic(std::unique_ptr<texture>& _tex) : tex{ std::move(_tex) } {}



	void scatter(const ray& _r_in, const hit_record& _rec, std::array<scatter_record, 3>& srec_) const override
	{
		srec_[1] = { ray(_rec.p, random_unit_vector(), _r_in.time()), tex->value(_rec.u, _rec.v, _rec.p), 1.0, true };
		srec_[0] = srec_[2] = { ray(_rec.p, vec3(0, 0, 0), _r_in.time()), color(0, 0, 0), 0.0, false };
		return;
	}

	bool is_equal(const material& _second) const override {
		const isotropic* o = dynamic_cast<const isotropic*>(&_second);
		return o && (tex == o->tex);
	}

private:
	std::unique_ptr<texture> tex;
};

#endif
