#pragma once


#include "onb.h"
#include "hittable_list.h"

class pdf
{
public:
	virtual ~pdf() {}

	virtual double value(const vec3& direction_) const = 0;
	virtual vec3 generate() const = 0;
};

class sphere_pdf : public pdf
{
public:
	sphere_pdf() {}

	double value(const vec3& direction_) const override;

	vec3 generate() const override;
};

class cosine_pdf : public pdf
{
public:
	cosine_pdf(const vec3& w_);

	double value(const vec3& direction_) const override;

	vec3 generate() const override;

private:
	onb uvw;
};

class hittable_pdf : public pdf
{
public:
	hittable_pdf(const hittable& objects_, const point3& origin_);

	double value(const vec3& direction_) const override;

	vec3 generate() const override;

private:
	const hittable& objects;
	point3 origin;
};