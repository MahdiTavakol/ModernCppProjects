#pragma once

#include "hittable_list.h"

struct triangle_struct
{
	std::array<point3, 3> vs;
	std::array<point2, 3> vts;
	std::array<point2, 3> vts_1;
	std::array<point3, 3> vns;

	vec3 unit_n1;
	vec3 w1;
	point3 Q1;
	double D1;

	int mat_indx;
	std::unordered_set<std::string> labels = { "all" };
};

class triangle_list : public hittable_list
{
public:
	triangle_list();
	triangle_list(std::unique_ptr<hittable> object);
	triangle_list(std::unique_ptr<hittable>& object);
	triangle_list(std::unique_ptr<hittable_list> list);

	virtual void clear() override;

	size_t size() const override;
	virtual hittable* operator[](const int& i) override;

	void return_params(
		std::vector<triangle_struct>& triangles_,
		std::vector<aabb>& bboxes_);

	aabb return_bbox(size_t i);

	std::vector<triangle_struct> return_triangles();

	std::vector<aabb> return_bboxes();

	std::vector<triangle_struct>& return_triangles_ref();

	std::vector<aabb>& return_bboxes_ref();

	std::unique_ptr<hittable_list> return_non_triangles();


	virtual void resize(const int size) override;
	virtual void sort() override;

	void sort_range(size_t& start_, size_t& end_);

	// backward compatiblity in the case the triangle object is putting into the list directly
	void add(std::unique_ptr<hittable> object) override;
	void add(
		const std::array<point3, 3>& vs_,
		const std::array<point2, 3>& vts_,
		const std::array<point2, 3>& vts_1_,
		const std::array<point3, 3>& vns_,
		const int& mat_indx_,
		const std::unordered_set<std::string> labels_ = {"all"});
	void add(
		const std::array<point3, 3>& vs_,
		const std::array<point2, 3>& vts_,
		const std::array<point3, 3>& vns_,
		const int& mat_indx_,
		const std::unordered_set<std::string> labels_ = { "all" });

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
	// this function should be called from the bvh as bvh in the assigment of nodes
	// will first determine if a ray hits the box of a triangle. This function 
	// does not check the bouding box for the cache efficiency purposes
	bool hit_indx(const ray& r_, interval ray_t, hit_record& rec_, size_t indx) const;
	// helper function to be used by the bvh_triangles to check if a triangle is hit
	static bool hit_triangle(const ray& r_, interval ray_t_, hit_record& rec_, const triangle_struct& tri_);

	virtual bool is_interior(double _a, double _b) const;
	static bool is_interior_static(double a_, double b_);
	static vec3 interpolate(double alpha_, double beta_, const std::array<point3, 3>& arr_);
	static vec2 interpolate(double alpha_, double beta_, const std::array<point2, 3>& arr_);

	virtual vec3 com() const override;

	virtual void scale(const vec3& center_, const double& factor_) override;

	void add_label(std::string label_) override;


	virtual aabb bounding_box(std::string label_, bool& set_) override;

private:
	std::vector<triangle_struct> triangles;
	std::vector<aabb> bboxes;

	// hittable_list for non_triangle objects
	std::unique_ptr<hittable_list> non_triangles;

	static aabb triangle_bbox(const std::array<point3, 3>& vs_);

	static inline bool box_compare(
		const aabb& a,
		const aabb& b,
		int axis_index
	)
	{
		auto a_axis_interval = a.axis_interval(axis_index);
		auto b_axis_interval = b.axis_interval(axis_index);
		return a_axis_interval.min < b_axis_interval.min;
	}

	static inline bool box_x_compare(
		const aabb& a,
		const aabb& b)
	{
		return box_compare(a, b, 0);
	}

	static inline bool box_y_compare(
		const aabb& a,
		const aabb& b)
	{
		return box_compare(a, b, 1);
	}

	static inline bool box_z_compare(
		const aabb& a,
		const aabb& b)
	{
		return box_compare(a, b, 2);
	}
};
