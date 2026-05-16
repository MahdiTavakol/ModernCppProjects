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

void hittable_list::resize(const int size_)
{
	objects.resize(size_);
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

size_t hittable_list::size() const
{
	size_t out = static_cast<size_t>(0);
	for (auto& obj : objects)
	{
		out += obj->size();
	}
	return out;
}

vec3 hittable_list::com()
{
	// each individual objects 
	// has a different com so for now
	// I am ignoring that 
	interval x = bbox.x;
	interval y = bbox.y;
	interval z = bbox.z;

	vec3 out{ (x.min + x.max) / 2.0,(y.min + y.max) / 2.0,(z.min + z.max) / 2.0 };
	return out;
}

vec3 hittable_list::com(std::string label_, bool& set_)
{
	aabb labelBox = bounding_box(label_, set_);

	if (set_ == false)
		return vec3();

	interval x = labelBox.x;
	interval y = labelBox.y;
	interval z = labelBox.z;

	vec3 out{ (x.min + x.max) / 2.0,(y.min + y.max) / 2.0,(z.min + z.max) / 2.0 };
	return out;
}

aabb hittable_list::bounding_box(std::string label_, bool& set_)
{
	aabb boxLabel = aabb::empty;
	for (auto& object : objects)
	{
		bool setObj = false;
		aabb boxObj = object->bounding_box(label_, setObj);
		set_ = set_ || setObj;
		if (setObj == true)
		{
			boxLabel = aabb(boxLabel, boxObj);
		}
	}

	if (set_ == true)
		return boxLabel;
	return aabb::empty;
}

void hittable_list::add_label(std::string label_)
{
	for (auto& object : objects)
	{
		object->add_label(label_);
	}
}

void hittable_list::scale(const vec3& center_, const double& factor_)
{
	for (auto& object : objects)
	{
		object->scale(center_, factor_);
	}
	bbox.scale(center_, factor_);
}
