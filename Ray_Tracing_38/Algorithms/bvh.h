#ifndef BVH_H
#define BVH_H

#include "../Geometry/aabb.h"
#include "../Geometry/hittable.h"
#include "../Algorithms/hittable_list.h"

#include <algorithm>

enum class BVH_Split_Method {
	NONE,
	MIDDLE,
	MEDIAN,
	SAH_SIMPLE,
};

class bvh_node : public hittable
{
public:
	bvh_node(std::unique_ptr<hittable_list> list,
		BVH_Split_Method split_method_ = BVH_Split_Method::MEDIAN);

	bvh_node(std::vector<std::unique_ptr<hittable>>& objects, size_t start, size_t end, 
		BVH_Split_Method split_method_ = BVH_Split_Method::MEDIAN);

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

	aabb bounding_box(std::string label_, bool& set_) override;

	material* get_material() override { return nullptr; }

	size_t size() const override;

	void scale(const vec3& center_, const double& factor_) override;

private:
	BVH_Split_Method split_mode;
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
