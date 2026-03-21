#pragma once

#include "../Shared/rtweekend.h"
#include "../Types/ray.h"
#include "../Types/vec3.h"

class material;

class hit_record
{
public:
    point3 p;
    vec3 normal;
    /* it does not own the material
     * The object owns it!
     */
    material* mat;
    double t;
    double u;
    double v;
    double w;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal);
};