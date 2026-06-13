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
		const int mat_indx_,
		std::string type_ = "mesh");
	mesh(const int mat_indx, std::string type_ = "mesh");
	virtual void initialize();
	virtual void set_bounding_box();
	aabb bounding_box() const override { return bbox; }
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
	virtual bool is_interior(double _a, double _b, hit_record& _rec) const;
	bool compare(hittable* rhs_, const double& tol) const override;
	virtual bool comparator(const std::unique_ptr<hittable>& rhs_) const override;
	virtual double get_area() const override;

protected:
	std::array<point3, 4> vs;
	std::array<point3, 4> vts;
	std::array<point3, 4> vns;
	

	int num_edges = 0;

	vec3 n1, n2;
	vec3 unit_n1, unit_n2;
	vec3 w1, w2;
	point3 Q1, Q2;
	double D1{ 0.0 }, D2{ 0.0 };

	// differences
	// edgeij = i the edge of j the triangle
	vec3 edge11, edge21, edge12, edge22;

	// the triangle indexes
	std::array<int, 3> ind1, ind2;

	static vec3 interpolate(double alpha_, double beta_, const std::array<point3,4>& arr_, std::array<int, 3> indx_);
};


#endif