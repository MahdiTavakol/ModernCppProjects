#ifndef HITTABLE_H
#define HITTABLE_H

#include "../Shared/rtweekend.h"
#include "aabb.h"
#include "interval.h"
#include "../Materials/material.h"
#include "../Types/ray.h"
#include "../Types/vec3.h"
#include "../Algorithms/hit_record.h"
#include <memory>



class hittable
{
public:
    hittable(std::string type_):
        type{type_}
    {}
    hittable(std::string type_, std::unique_ptr<material> mat_):
        type{type_}, mat{std::move(mat_)}
    {}
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
    virtual aabb bounding_box() const
    {
        return bbox;
    }


    virtual bool compare(hittable* rhs_, const double& tol_) const
    {
        // it means that the compare has not been implemented
        std::cout << "Warning: The compare function has not been implemented for this object type yet!" << std::endl;
        return true;
    }

    virtual material* get_material();
    virtual bool comparator(const std::unique_ptr<hittable>& rhs_) const;
    int typeCompare(const hittable& rhs_) const;
    const std::string type = "";
protected:
    std::unique_ptr<material> mat;
    aabb bbox;
};

class translate : public hittable
{
public:
    translate(std::unique_ptr<hittable> _object, const vec3& _offset);

    bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;

    aabb bounding_box() const override { return bbox; }

    virtual material* get_material() override {
        return object->get_material();
    }

private:
    std::unique_ptr<hittable> object;
    vec3 offset;
};

class rotate_y : public hittable
{
public:
    rotate_y(std::unique_ptr<hittable> _object, double _angle);

    bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;

    aabb bounding_box() const override { return bbox; }

    virtual material* get_material() override {
        return object->get_material();
    }

private:
    std::unique_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
};

#endif
