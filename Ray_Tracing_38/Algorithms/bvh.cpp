#include "bvh.h"

bvh_node::bvh_node(std::string type_, BVH_Split_Method split_method_) :
	hittable{type_},split_mode{split_method_}
{
}

// this one steals the std::vector<std::unique_ptr<hittable>
// object from the input list
bvh_node::bvh_node(std::unique_ptr<hittable_list> list, BVH_Split_Method split_mode_) :
	bvh_node(list->objects, 0, list->objects.size(), split_mode_)
{}

bvh_node::bvh_node(std::vector<std::unique_ptr<hittable>>& objects, size_t start, size_t end, 
	BVH_Split_Method split_mode_):
	hittable{"bvh_node"}, split_mode{split_mode_}
{
	bbox = aabb::empty;

	for (size_t object_index = start; object_index < end; object_index++)
		bbox = aabb(bbox, objects[object_index]->bounding_box());


	size_t object_span = end - start;

	if (object_span == 0)
	{
		left = std::make_unique<hittable_list>();
		right = std::make_unique<hittable_list>();
	}
	else if (object_span == 1) {
		left = std::move(objects[start]);
		// empty object list
		right = std::make_unique<hittable_list>();
	}
	else if (object_span == 2) {
		right = std::move(objects[start]);
		left = std::move(objects[start + 1]);
	}
	else {
		int axis = bbox.longest_axis();
		auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;
		std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);


		switch (split_mode)
		{
		case BVH_Split_Method::MEDIAN:
			{
			auto mid = start + object_span / 2;
			left = std::make_unique<bvh_node>(objects, start, mid);
			right = std::make_unique<bvh_node>(objects, mid, end);
			break;
			}
			case BVH_Split_Method::SAH_SIMPLE:
			{
				int len = end - start;

				std::vector<aabb> left_boxes(len);
				std::vector<aabb> right_boxes(len);

				aabb bbox_left = aabb::empty;
				aabb bbox_right = aabb::empty;

				left_boxes[0] = bbox_left;

				// the left does not include the divider element
				// while right includes that element.

				for (int i = 1; i < len; i++)
				{
					bbox_left = aabb(bbox_left, objects[start + i - 1]->bounding_box());
					left_boxes[i] = bbox_left;
				}

				for (int i = len-1; i >= 0; i--)
				{
					bbox_right = aabb(bbox_right, objects[start + i]->bounding_box());
					right_boxes[i] = bbox_right;
				}

				std::vector<double> Cfactors(len);
				// splits with at least one element on each side
				for (int i = 1; i < len; i++)
				{
					const aabb& left = left_boxes[i];
					const aabb& right = right_boxes[i];
					double left_area = left.surface_area();
					double right_area = right.surface_area();
					Cfactors[i] = i * left_area + (len - i) * right_area;
				}
				auto min_iter = std::min_element(Cfactors.begin()+1, Cfactors.end());
				int mid = std::distance(Cfactors.begin(), min_iter) + start;
				left = std::make_unique<bvh_node>(objects, start, mid);
				right = std::make_unique<bvh_node>(objects, mid, end);
				break;
			}
		}

	}

}

bool bvh_node::hit(const ray& r, interval ray_t, hit_record& rec) const 
{
	if (!bbox.hit(r, ray_t))
		return false;

	bool hit_left = left->hit(r, ray_t, rec);
	bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);
	return hit_left || hit_right;
}

aabb bvh_node::bounding_box(std::string label_, bool& set_)
{
	bool leftSet = false;
	bool rightSet = false;
	aabb leftBox = left->bounding_box(label_, leftSet);
	aabb rightBox = right->bounding_box(label_, rightSet);

	set_ = leftSet || rightSet;
	if (!set_)
		return aabb();
	if (leftSet && rightSet)
	{
		return aabb(rightBox, leftBox);
	}
	else if (leftSet)
	{
		return leftBox;
	}
	else if (rightSet)
	{
		return rightBox;
	}
	return aabb();
}


bool bvh_node::box_compare(
	const std::unique_ptr<hittable>& a,
	const std::unique_ptr<hittable>& b, 
	int axis_index
	)
{
	auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
	auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
	return a_axis_interval.min < b_axis_interval.min;
}

size_t bvh_node::size() const
{
	return left->size() + right->size();
}

void bvh_node::scale(const vec3& center_, const double& factor_)
{
	left->scale(center_, factor_);
	right->scale(center_, factor_);
	bbox.scale(center_, factor_);
}




