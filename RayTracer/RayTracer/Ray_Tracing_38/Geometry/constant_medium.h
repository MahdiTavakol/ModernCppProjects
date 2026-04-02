#ifndef CONSTANT_MEDIUM
#define CONSTANT_MEDIUM

#include "../Geometry/hittable.h"
#include "../Materials/material.h"
#include "texture.h"

class constant_medium : public hittable {
public:
	constant_medium(std::unique_ptr<hittable> _boundary, 
		double _density, std::unique_ptr<texture>& _tex);

	constant_medium(std::unique_ptr<hittable> _boundary,
		double _density, const color& _albedo);

	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;


private:
	std::unique_ptr<hittable> boundary;
	double neg_inv_density;
	std::unique_ptr<material> phase_function;
};

#endif