#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
#include "../Algorithms/hittable_list.h"
#include "../Materials/material.h"

class quad : public hittable {
public:
    quad(const point3& Q_, const vec3& u_, const vec3& v_,
        const int mat_indx_, std::string type_ = "quad");
	virtual void set_bounding_box();
	aabb bounding_box() const override { return bbox; }
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
    bool compare(hittable* rhs_, const double& tol_) const override;
    virtual bool comparator(const std::unique_ptr<hittable>& rhs_) const override;
	// Since virtual is not allowed with template functions I had to have two similar definitions of is_interior.
	virtual bool is_interior(double _a, double _b, hit_record& _rec) const;
    double get_area() const override;


protected:
	point3 Q;
	vec3 u, v;
	vec3 w;
	vec3 normal;
	double D;
};

std::unique_ptr<hittable_list> box(const point3& a, const point3& b, const int& mat_indx_);
std::unique_ptr<hittable_list> box(const aabb& bbox, const int& mat_indx_);


#endif
