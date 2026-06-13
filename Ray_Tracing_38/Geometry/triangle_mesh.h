#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include <array>
#include "../Types/vec3.h"
#include "../Geometry/hittable.h"
#include "../Materials/material.h"


class triangle_mesh : public hittable {
public:

	triangle_mesh(const std::array<point3, 3>& vs_,
		          const std::array<point2, 3>& vts_,
		          const std::array<point3, 3>& vns_,
		          const int mat_indx_);
	triangle_mesh(const std::array<point3,3>& v_,
		          const std::array<point2,3>& vts_,
				  const std::array<point2,3>& vts_1_,
		          const std::array<point3,3>& vn_,
		          const int mat_indx_);
	virtual void initialize();
	virtual void set_bounding_box();
	void flip();
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
	virtual bool is_interior(double _a, double _b, hit_record& _rec) const;
	void return_params(
		std::array<point3, 3>& vs_,
		std::array<point2, 3>& vts_,
		std::array<point2,3>& vts_1_,
		std::array<point3, 3>& vns_);
	bool compare(hittable* rhs_, const double& tol) const override;
	virtual bool comparator(const std::unique_ptr<hittable>& rhs_) const override;
	double get_area() const override;
	static vec3 interpolate(double alpha_, double beta_, const std::array<point3, 3>& arr_);
	static vec2 interpolate(double alpha_, double beta_, const std::array<point2, 3>& arr_);
	void scale(const vec3& center_, const double& factor_) override;

protected:
	std::array<point3, 3> vs;
	std::array<point2, 3> vts;
	std::array<point2, 3> vts_1;
	std::array<point3, 3> vns;

	int num_edges;


	vec3 n1;
	vec3 unit_n1;
	vec3 w1;
	point3 Q1;
	double D1;
};


#endif