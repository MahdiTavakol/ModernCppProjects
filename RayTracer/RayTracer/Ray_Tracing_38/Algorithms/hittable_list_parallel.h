#ifndef HITTABLE_LIST_PARALLEL_H
#define HITTABLE_LIST_PARALLEL_H

#include "hittable_list.h"
#include "mpi.h"

struct hit_record_struct
{
	point3 p;
	vec3 normal;
	double t{ 0.0 };
	double u{ 0.0 };
	double v{ 0.0 };
	double w{ 0.0 };
	bool front_face = false;
};

hit_record_struct hit_record_to_struct(const hit_record&);


class hittable_list_parallel : public hittable_list {
public:

	void add(std::unique_ptr<hittable>& object)
	{
		objects.push_back(std::move(object));
		//bbox = aabb(bbox, object->bounding_box());
	}


	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;


protected:
	aabb bbox;
};

#endif
