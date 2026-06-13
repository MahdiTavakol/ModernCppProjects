#ifndef MATERIAL_H
#define MATERIAL_H


#include "../Shared/rtweekend.h"

#include "../Types/ray.h"
#include "../Types/color.h"
#include "../Geometry/texture.h"
#include "../Algorithms/hit_record.h"
#include <array>
#include <vector>
#include <string>
#include "../Output/Logger.h"

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
	material(Logger* error_);
	virtual ~material() = default;


	virtual color emitted([[maybe_unused]] double _u, [[maybe_unused]] double _v, [[maybe_unused]] const point3& _p) const;
	virtual void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const;
	virtual bool is_equal(const material& _second) const = 0;
	virtual bool compare(material* _rhs, const double tol_) const;
	bool operator==(const material& _second);

protected:
	Logger* error;

};

// general material class for mtl format material
class general : public material
{
public:
	general(
		Logger* error,
		const color& _albedo, const double& _shininess,
		const double& _Tr, const color& _Tf, const color _Ks, const double _Ni);


	void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const override;
	void diffuse_scatter(const ray& r_in, const hit_record& rec, scatter_record& srec_, double& weight_) const;
	void specular_scatter(const ray& r_in, const hit_record& rec, scatter_record& srec_, double& weight_) const;
	void transmit_scatter(const ray& r_in, const hit_record& rec, scatter_record& srec_, double& weight_) const;
	bool is_equal(const material& _second) const override;
	bool compare(material* rhs_, const double tol_) const override;



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
	lambertian(Logger* error_, const color& _albedo);
	lambertian(Logger* error_, std::unique_ptr<texture> _tex);

	void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const override;
	bool is_equal(const material& _second) const override;

private:
	std::unique_ptr<texture> tex;
};

class metal : public material
{
public:
	metal(Logger* error_);
	metal(Logger *error_, const color& _albedo, double _fuzz);

	void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const override;

	void return_params(color& _albedo, double& _fuzz);

	bool is_equal(const material& _second) const override;

private:
	color albedo;
	double fuzz;
};

class dielectric : public material
{
public:
	dielectric(Logger* error_, double _refraction_index, color attenuation_ = color(1.0, 1.0, 1.0));


	void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const override;

	void return_params(double& _ref);

	bool is_equal(const material& _second) const override;

private:
	color attenuation;
	double refraction_index;

	static double reflectance(double cosine, double refraction_index);
};


class diffuse_light : public material {
public:
	diffuse_light(Logger* error_, std::unique_ptr<texture>& _tex);
	diffuse_light(Logger* error_, const color& _emit);


	color emitted(double _u, double _v, const point3& _p)  const override;

	bool is_equal(const material& _second) const override;

private:
	std::unique_ptr<texture> tex;
};

class isotropic : public material
{
public:
	isotropic(Logger* error_, const color& _albedo);
	isotropic(Logger* error_, std::unique_ptr<texture>& _tex);



	void scatter(const ray& _r_in, const hit_record& _rec, std::array<scatter_record, 3>& srec_) const override;

	bool is_equal(const material& _second) const override;

private:
	std::unique_ptr<texture> tex;
};

#endif
