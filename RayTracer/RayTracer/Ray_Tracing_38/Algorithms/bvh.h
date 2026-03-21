#ifndef BVH_H
#define BVH_H

#include "../Geometry/aabb.h"
#include "../Geometry/hittable.h"
#include "../Algorithms/hittable_list.h"

#include <algorithm>

class bvh_node : public hittable
{
public:
	bvh_node(std::unique_ptr<hittable_list> list);

	bvh_node(std::vector<std::unique_ptr<hittable>>& objects, size_t start, size_t end);

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

	material* get_material() override { return nullptr; }

private:
	std::unique_ptr<hittable> left;
	std::unique_ptr<hittable> right;

	static bool box_compare(
		const std::unique_ptr<hittable>& a, 
		const std::unique_ptr<hittable>& b, 
		int axis_index
	);

	static bool box_x_compare(
		const std::unique_ptr<hittable>& a,
		const std::unique_ptr<hittable>& b)
	{
		return box_compare(a, b, 0);
	}

	static bool box_y_compare(
		const std::unique_ptr<hittable>& a, 
		const std::unique_ptr<hittable>& b)
	{
		return box_compare(a, b, 1);
	}

	static bool box_z_compare(
		const std::unique_ptr<hittable>& a,
		const std::unique_ptr<hittable>& b)
	{
		return box_compare(a, b, 2);
	}
};


#endif
