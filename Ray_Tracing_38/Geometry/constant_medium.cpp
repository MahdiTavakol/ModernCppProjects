#include "constant_medium.h"
#include <iostream>


constant_medium::constant_medium(
	std::unique_ptr<hittable> boundary_,
	double density_, 
	int fog_mat_indx_)
	:
	hittable{ "constant_medium",fog_mat_indx_ },
	boundary{ std::move(boundary_) },
	neg_inv_density{ -1.0 / density_ }
{}


bool constant_medium::hit(const ray& _r, interval _ray_t, hit_record& _rec) const  {
	hit_record rec1, rec2;

	// entering the boundary: respect the incoming ray interval
	if (!boundary->hit(_r, _ray_t, rec1))
		return false;

	// leaving the boundary
	// search for the second hit after the first; limit to the ray's max
	if (!boundary->hit(_r, interval(rec1.t + 0.0001, _ray_t.max), rec2))
		return false;
		

	// clamp the entry/exit to the provided ray interval
	if (rec1.t < _ray_t.min) rec1.t = _ray_t.min;
	if (rec2.t > _ray_t.max) rec2.t = _ray_t.max;
	
	if (rec1.t >= rec2.t)
		return false;

	// if the entry is before the ray origin, clamp to ray min
	if (rec1.t < _ray_t.min)
		rec1.t = _ray_t.min;
		
	auto ray_length = _r.direction().length();
	auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
	auto hit_distance = neg_inv_density* std::log(random_double());
	if (hit_distance > distance_inside_boundary)
		return false;

	_rec.t = rec1.t +hit_distance / ray_length;
	_rec.p = _r.at(_rec.t);

	_rec.normal = vec3(1, 0, 0);
	_rec.front_face =  true;
	_rec.mat_indx = mat_indx;

	return true;
}

aabb constant_medium::bounding_box(std::string label_, bool& set_)
{
	aabb out =  boundary->bounding_box(label_, set_);
	return out;
 }