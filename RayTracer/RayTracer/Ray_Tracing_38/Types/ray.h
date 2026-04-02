#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	ray() {}

	ray(const point3& origin, const vec3& direction, double time): 
		orig(origin), dir(direction), tm(time) {}

	ray(const point3& origin, const vec3& direction)
		:ray(origin, direction, 0) {}
	const point3& origin() const { return orig; }
	const vec3& direction() const { return dir; }

	double time() const { return tm; }

	point3 at(double t) const {
		return orig + t * dir;
	}


private:
	point3 orig;
	vec3 dir;
	double tm{ 0.0 };
};

inline ray operator-(const ray& lhs_, const ray& rhs_)
{
	return ray(lhs_.origin() - rhs_.origin(), lhs_.direction() - rhs_.direction(), lhs_.time() - rhs_.time());
}

#endif