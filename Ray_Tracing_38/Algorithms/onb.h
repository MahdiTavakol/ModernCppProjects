#pragma once
#include "../Types/vec3.h"

class onb
{
public:
	onb(const vec3& n_);

	const vec3& u() const;
	const vec3& v() const;
	const vec3& w() const;

	vec3 transform(const vec3& v_) const;

private:
	vec3 axis[3];
};