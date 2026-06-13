#ifndef SPHERE_H
#define SPHERE_H

#include "../Shared/rtweekend.h"

#include "hittable.h"
#include "../Types/vec3.h"
#include "../Types/ray.h"
#include "interval.h"
#include "aabb.h"
#include "../Materials/material.h"
#include <memory>
#include <algorithm>

class sphere : public hittable {
public:
    // Stationary Sphere
    sphere(const point3& _center,
        double _radius,
        const int mat_indx);

    // Moving Sphere
    sphere(const point3& _center1,
        const point3& _center2,
        double _radius,
        const int mat_indx);

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
    virtual bool compare(hittable* rhs_, const double& tol_) const override;
    virtual bool comparator(const std::unique_ptr<hittable>& rhs_) const override;

    double get_radius();



private:
    ray center;
    double radius;
    

    static void get_sphere_uv(const point3& p, double& u, double& v);
};

#endif
