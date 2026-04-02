#include "hittable_list.h"

hittable_list::hittable_list(std::unique_ptr<hittable> object):
	hittable{"hittable_list"}
{
	add(std::move(object));
}

hittable_list::hittable_list(std::unique_ptr<hittable>& object):
	hittable{"hittable_list"}
{
	add(std::move(object)); 
}

bool hittable_list::hit(const ray& r, interval ray_t, hit_record& rec) const 
{
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = ray_t.max;

	for (const auto& object : objects)
	{
		if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

void hittable_list::sort()
{
	auto sortingLambda = 
		[](const std::unique_ptr<hittable>& a_, const std::unique_ptr<hittable>& b_)
		-> bool
	{
		// first sort by shape type
		if (a_->type < b_->type)
			return true;
		else if (a_->type > b_->type)
			return false;
		// then if the types are the same
		// use the objects own comparator function.
		return a_->comparator(b_);
	};

	std::sort(objects.begin(), objects.end(),sortingLambda);
}

void hittable_list::add(std::unique_ptr<hittable> object)
{
	bbox = aabb(bbox, object->bounding_box());
	objects.push_back(std::move(object));
}
