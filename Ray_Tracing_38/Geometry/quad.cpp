#include "quad.h"


quad::quad(const point3& Q_, const vec3& u_, const vec3& v_,
	const int mat_indx_, std::string type_) :
	hittable{ type_,mat_indx_ },
	Q{ Q_ }, u{ u_ }, v{ v_ }
{
	auto n = cross(u_, v_);
	normal = unit_vector(n);
	D = dot(normal, Q);
	w = n / dot(n, n);

	set_bounding_box();
}

void quad::set_bounding_box()
{
	auto bbox_diagonal1 = aabb(Q, Q + u + v);
	auto bbox_diagonal2 = aabb(Q + u, Q + v);
	bbox = aabb(bbox_diagonal1, bbox_diagonal2);
}

bool quad::compare(hittable* rhs_, const double& tol_) const
{
	quad* rhsConv = dynamic_cast<quad*>(rhs_);
	// testing the type of the rhs
	if (!rhsConv)
		throw std::invalid_argument("The rhs_ is not of type quad");

	auto deltaU = u - rhsConv->u;
	auto deltaV = v - rhsConv->v;
	// we do not care about comparing Q values
	double distU = deltaU.length();
	double distV = deltaV.length();

	if (distU >= tol_)
		return true;
	if (distV >= tol_)
		return true;
	return false;
}

bool quad::comparator(const std::unique_ptr<hittable>& rhs_) const
{
	auto rhs_cast = dynamic_cast<quad*>(rhs_.get());
	if (!rhs_cast)
		throw std::invalid_argument("Different types!");

	double area = this->get_area();
	double rhs_area = rhs_cast->get_area();

	if (area < rhs_area)
		return true;
	return false;
}

bool quad::hit(const ray& _r, interval _ray_t, hit_record& _rec) const
{
	auto denom = dot(normal, _r.direction());

	if (std::fabs(denom) < 1e-8)
		return false;

	auto t = (D - dot(normal, _r.origin())) / denom;
	if (!_ray_t.contains(t))
		return false;

	auto intersection = _r.at(t);
	vec3 planar_hitpt_vector = intersection - Q;
	auto alpha = dot(w, cross(planar_hitpt_vector, v));
	auto beta = dot(w, cross(u, planar_hitpt_vector));

	if (!is_interior(alpha, beta, _rec))
		return false;

	_rec.t = t;
	_rec.p = intersection;
	_rec.mat_indx = mat_indx;
	_rec.set_face_normal(_r, normal);

	return true;
}


bool quad::is_interior(double _a, double _b, hit_record& _rec) const {
	interval unit_interval = interval(0, 1);

	if (!unit_interval.contains(_a) || !unit_interval.contains(_b))
		return false;

	_rec.u = _a;
	_rec.v = _b;
	return true;
}

double quad::get_area() const {
	auto crs = cross(u, v);
	double area = crs.length();
	return area;
}


std::unique_ptr<hittable_list> box(const point3& a, const point3& b, const int& mat_indx_)
{
	auto sides = std::make_unique<hittable_list>();

	// Construct the two opposite vertices with the minimum and maximum coordinates.
	auto min = point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
	auto max = point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

	auto dx = vec3(max.x() - min.x(), 0, 0);
	auto dy = vec3(0, max.y() - min.y(), 0);
	auto dz = vec3(0, 0, max.z() - min.z());


	sides->add(std::make_unique<quad>(point3(min.x(), min.y(), max.z()), dx, dy, mat_indx_)); // front
	sides->add(std::make_unique<quad>(point3(max.x(), min.y(), max.z()), -dz, dy, mat_indx_)); // right
	sides->add(std::make_unique<quad>(point3(max.x(), min.y(), min.z()), -dx, dy, mat_indx_)); // back
	sides->add(std::make_unique<quad>(point3(min.x(), min.y(), min.z()), dz, dy, mat_indx_)); // left
	sides->add(std::make_unique<quad>(point3(min.x(), max.y(), max.z()), dx, -dz, mat_indx_)); // top
	sides->add(std::make_unique<quad>(point3(min.x(), min.y(), min.z()), dx, dz, mat_indx_)); // bottom


	return sides;
}

std::unique_ptr<hittable_list> box(const aabb& bbox, const int& mat_indx_)
{
	vec3 dx = vec3{ bbox.x.size(),0.0,0.0 };
	vec3 dy = vec3{ 0.0,bbox.y.size(),0.0 };
	vec3 dz = vec3{ 0.0,0.0,bbox.z.size() };

	auto sides = std::make_unique<hittable_list>();

	sides->add(std::make_unique<quad>(point3(bbox.x.min, bbox.y.min, bbox.z.max), dx, dy, mat_indx_)); // front
	sides->add(std::make_unique<quad>(point3(bbox.x.max, bbox.y.min, bbox.z.max), -dz, dy, mat_indx_)); // right
	sides->add(std::make_unique<quad>(point3(bbox.x.max, bbox.y.min, bbox.z.min), -dx, dy, mat_indx_)); // back
	sides->add(std::make_unique<quad>(point3(bbox.x.min, bbox.y.min, bbox.z.min), dz, dy, mat_indx_)); // left
	sides->add(std::make_unique<quad>(point3(bbox.x.min, bbox.y.max, bbox.z.max), dx, -dz, mat_indx_)); // top
	sides->add(std::make_unique<quad>(point3(bbox.x.min, bbox.y.min, bbox.z.min), dx, dz, mat_indx_)); // bottom


	return sides;

}

