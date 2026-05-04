#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	ray() {}

	ray(const point3& origin, const vec3& direction, double time): 
		orig(origin), 
		dir(direction),
		tm(time)
	{
		dir_inv[0] = 1.0 / dir[0];
		dir_inv[1] = 1.0 / dir[1];
		dir_inv[2] = 1.0 / dir[2];
	}

	ray(const point3& origin, const vec3& direction)
		:ray(origin, direction, 0) {}
	const point3& origin() const { return orig; }
	const vec3& direction() const { return dir; }
	const vec3& direction_inv() const { return dir_inv; }

	double time() const { return tm; }

	point3 at(double t) const {
		return orig + t * dir;
	}


private:
	point3 orig;
	vec3 dir;
	vec3 dir_inv;
	double tm{ 0.0 };
};

inline ray operator-(const ray& lhs_, const ray& rhs_)
{
	return ray(lhs_.origin() - rhs_.origin(), lhs_.direction() - rhs_.direction(), lhs_.time() - rhs_.time());
}

#endif