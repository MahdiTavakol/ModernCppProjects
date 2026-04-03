#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
#include "../Algorithms/hittable_list.h"
#include "../Materials/material.h"

class quad : public hittable {
public:
	quad(const point3& _Q, const vec3& _u, const vec3& _v,
        std::unique_ptr<material> _mat, std::string type_="quad");
	virtual void set_bounding_box();
	aabb bounding_box() const override { return bbox; }
	bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;
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

inline std::unique_ptr<hittable_list> box(const point3& a, const point3& b, 
    std::vector<std::unique_ptr<material>>& matVec)
{
    if (matVec.size() != 6)
        std::cout << "Error not enough materials in box" << std::endl;
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = std::make_unique<hittable_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
    auto max = point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());


    sides->add(std::make_unique<quad>(point3(min.x(), min.y(), max.z()), dx, dy, std::move(matVec[0]))); // front
    sides->add(std::make_unique<quad>(point3(max.x(), min.y(), max.z()), -dz, dy, std::move(matVec[1]))); // right
    sides->add(std::make_unique<quad>(point3(max.x(), min.y(), min.z()), -dx, dy, std::move(matVec[2]))); // back
    sides->add(std::make_unique<quad>(point3(min.x(), min.y(), min.z()), dz, dy, std::move(matVec[3]))); // left
    sides->add(std::make_unique<quad>(point3(min.x(), max.y(), max.z()), dx, -dz, std::move(matVec[4]))); // top
    sides->add(std::make_unique<quad>(point3(min.x(), min.y(), min.z()), dx, dz, std::move(matVec[5]))); // bottom

    return sides;
}


#endif
