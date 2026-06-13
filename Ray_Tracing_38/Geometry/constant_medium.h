#ifndef CONSTANT_MEDIUM
#define CONSTANT_MEDIUM

#include "../Geometry/hittable.h"
#include "../Algorithms/hittable_list.h"


class constant_medium : public hittable {
public:
	constant_medium(
		std::unique_ptr<hittable> shape_,
		double density_,
		int fog_mat_indx_);
	constant_medium(
		hittable* boundary_,
		double density_,
		int fog_mat_indx_);
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;

	aabb bounding_box(std::string label_, bool& set_) override;


private:
	std::unique_ptr<hittable_list> shapes;
	hittable* boundary;
	double neg_inv_density;
};

#endif