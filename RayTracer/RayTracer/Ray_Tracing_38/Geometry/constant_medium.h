#ifndef CONSTANT_MEDIUM
#define CONSTANT_MEDIUM

#include "../Geometry/hittable.h"
#include "../Materials/material.h"
#include "../Materials/material_list.h"
#include "texture.h"

class constant_medium : public hittable {
public:

	constant_medium(std::unique_ptr<hittable> boundary_,
		double density_, std::unique_ptr<texture>& tex_,
		material_list& list_);
	constant_medium(std::unique_ptr<hittable> boundary_,
		double density_, const color& albedo_,
		material_list& list_);
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;

	aabb bounding_box(std::string label_, bool& set_) override;


private:
	std::unique_ptr<hittable> boundary;
	double neg_inv_density;
};

#endif