#include "pdf.h"

double sphere_pdf::value(const vec3& direction_) const
{
	return 1.0 / (4.0 * pi);
}

vec3 sphere_pdf::generate() const
{
	return random_unit_vector();
}

cosine_pdf::cosine_pdf(const vec3& w_):
	uvw{ w_ }
{ }

double cosine_pdf::value(const vec3& direction_) const
{
	auto cosine_theta = dot(unit_vector(direction_), uvw.w());
	return std::fmax(0, cosine_theta / pi);
}

vec3 cosine_pdf::generate() const
{
	return uvw.transform(vec3::random_cosine_direction());
}

hittable_pdf::hittable_pdf(const hittable& objects_, const point3& origin_):
	objects{objects_}, origin{origin_}
{}

double hittable_pdf::value(const vec3& direction_) const
{
	return objects.pdf_value(origin, direction_);
}

vec3 hittable_pdf::generate() const
{
	return objects.random(origin);
}