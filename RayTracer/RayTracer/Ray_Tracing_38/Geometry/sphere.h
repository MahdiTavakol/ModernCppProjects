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
           std::unique_ptr<material> _mat);

    // Moving Sphere
    sphere(const point3& _center1, 
           const point3& _center2,
           double _radius, 
           std::unique_ptr<material> _mat);

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;



    aabb bounding_box() const override { return bbox; }



private:
    ray center;
    double radius;
    

    static void get_sphere_uv(const point3& p, double& u, double& v);
};

#endif
