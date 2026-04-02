#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "mesh.h"
#include <array>

class triangle_mesh final : public mesh {
public:
	triangle_mesh(const std::array<point3, 3>& _vs,
		          const std::array<point3, 3>& _vts,
		          const std::array<point3, 3>& _vns,
		          std::unique_ptr<material> _mat);
	virtual void initialize() override;
	virtual void set_bounding_box() override;
	aabb bounding_box() const override { return bbox; }
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
	virtual bool is_interior(double _a, double _b, hit_record& _rec) const override;
	void return_params(
		std::array<point3, 3>& vs_,
		std::array<point3, 3>& vts_,
		std::array<point3, 3>& vns_,
		material* mat_);
	bool compare(hittable* rhs_, const double& tol) const override;
	virtual bool comparator(const std::unique_ptr<hittable>& rhs_) const override;

protected:
	std::array<point3, 3> vs;
	std::array<point3, 3> vts;
	std::array<point3, 3> vns;

	int num_edges;

	aabb bbox;
	vec3 n1;
	vec3 unit_n1;
	vec3 w1;
	point3 Q1;
	double D1;

};


#endif