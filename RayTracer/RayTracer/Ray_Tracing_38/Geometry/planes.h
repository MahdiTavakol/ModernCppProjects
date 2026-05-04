#ifndef PLANES_H
#define PLANES_H

#include "quad.h"

class triangle final : public quad
{
public:
	triangle(const point3& _Q, const vec3& _u, const vec3& _v, 
		std::unique_ptr<material> _mat);
	triangle(const point3& _Q, const vec3& _u, const vec3& _v,
		const int mat_indx_);
	bool is_interior(double _a, double _b, hit_record& _rec) const override;

};

class circle final : public quad
{
public:
	circle(const point3& _Q, const vec3& _u, const vec3& _v, 
		std::unique_ptr<material> _mat);
	circle(const point3& _Q, const vec3& _u, const vec3& _v,
		const int mat_indx);
	bool is_interior(double _a, double _b, hit_record& _rec) const override;
};


#endif
