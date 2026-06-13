#ifndef AABB_H
#define AABB_H

#include "../Shared/rtweekend.h"
#include "interval.h"
#include "../Types/vec3.h"
#include "../Types/ray.h"
#include <array>

class aabb {
public:
	interval x, y, z;

	aabb() {}

	aabb(const interval& _x, const interval& _y, const interval& _z);

	aabb(const point3& a, const point3& b);

	aabb(const aabb& box0, const aabb& box1);

	const interval& axis_interval(int n) const;

	bool hit(const ray& r, interval ray_t) const;


	int longest_axis() const;

	double surface_area() const;
	
	static const aabb empty, universe;

	aabb operator+(const vec3& _offset)
	{
		return aabb(this->x + _offset.x(), this->y + _offset.y(), this->z + _offset.z());
	}


	void scale(const vec3 center_, double factor_);
	
private:
	void pad_to_minimums();
	void sort_axis();

	std::array<int, 3> axises;
};


#endif
