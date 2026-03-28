#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "mesh.h"

class triangle_mesh : public mesh {
public:
	triangle_mesh(const std::vector<point3>& _vs, 
		          const std::vector<point3>& _vts, 
		          const std::vector<point3>& _vns, 
		          std::unique_ptr<material> _mat);
	virtual void initialize() override;
	virtual void set_bounding_box() override;
	aabb bounding_box() const override { return bbox; }
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
	virtual bool is_interior(double _a, double _b, hit_record& _rec) const override;
	void return_params(
		std::vector<point3>& vs_,
		std::vector<point3>& vts_, 
		std::vector<point3>& vns_, 
		material* mat_);
	bool compare(hittable* rhs_, const double& tol) const override;

protected:
	std::vector<point3> vs;
	std::vector<point3> vts;
	std::vector<point3> vns;

	int num_edges;

	aabb bbox;
	vec3 unit_n1;
	vec3 w1;
	point3 Q1;
	double D1;

};


#endif