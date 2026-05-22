#ifndef MATERIAL_H
#define MATERIAL_H


#include "../Shared/rtweekend.h"

#include "../Types/ray.h"
#include "../Types/color.h"
#include "../Geometry/texture.h"
#include "../Algorithms/hit_record.h"
#include <array>

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
			specular_strength = 1.0;
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
