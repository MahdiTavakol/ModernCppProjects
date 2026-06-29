#include "onb.h"

onb::onb(const vec3& n_)
{
	axis[2] = unit_vector(n_);
	vec3 a = (std::abs(axis[2].x()) > 0.9) ? vec3{ 0,1,0 } : vec3{ 1,0,0 };
	axis[1] = unit_vector(cross(axis[2], a));
	axis[0] = cross(axis[2], axis[1]);
}

const vec3& onb::u() const
{
	return axis[0];
}

const vec3& onb::v() const
{
	return axis[1];
}

const vec3& onb::w() const
{
	return axis[2];
}

vec3 onb::transform(const vec3& v_) const
{
	return (v_[0] * axis[0]) + (v_[1] * axis[1]) + (v_[2] * axis[2]);
}