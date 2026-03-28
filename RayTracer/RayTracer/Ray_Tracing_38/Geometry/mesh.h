#ifndef MESH_H
#define MESH_H

#include <array>

#include "hittable.h"
#include "../Algorithms/hittable_list.h"
#include "quad.h"

class mesh : public hittable {
public:
	mesh(
		const std::array<point3, 4>& vs,
		const std::array<point3, 4>& vts,
		const std::array<point3, 4>& vns,
		std::unique_ptr<material> _mat);
	mesh() {}
	virtual void initialize();
	virtual void set_bounding_box();
	aabb bounding_box() const override { return bbox; }
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
	virtual bool is_interior(double _a, double _b, hit_record& _rec) const;
	bool compare(hittable* rhs_, const double& tol) const override;

protected:
	std::array<point3, 4> vs;
	std::array<point3, 4> vts;
	std::array<point3, 4> vns;
	

	int num_edges = 0;

	vec3 unit_n1, unit_n2;
	vec3 w1, w2;
	point3 Q1, Q2;
	double D1{ 0.0 }, D2{ 0.0 };

	static vec3 interpolate(const std::vector<point3>& _triangle, const std::vector<point3>& _normals, const vec3& _point);
};


#endif