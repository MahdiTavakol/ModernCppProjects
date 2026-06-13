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
#include <unordered_set>



class hittable
{
public:
    hittable(std::string type_):
        type{type_}
    {}
    hittable(std::string type_, const int mat_indx_):
        type{type_}, mat_indx{mat_indx_}
    {}
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
    virtual aabb bounding_box() const
    {
        return bbox;
    }


    virtual bool compare(hittable* rhs_, const double& tol_) const;

    virtual bool comparator(const std::unique_ptr<hittable>& rhs_) const;
    int typeCompare(const hittable& rhs_) const;
    virtual double get_area() const
    {
        return 0;
    }

    virtual size_t size() const
    {
        return static_cast<size_t>(1);
    }

    
    virtual aabb bounding_box(std::string label_, bool& set_);
    virtual void add_label(std::string label_);
    virtual void scale(const vec3& center_, const double& factor_);
    int return_mat_indx() { return mat_indx; }
    std::unordered_set<std::string> return_labels() { return labels; }

    const std::string type = "";
protected:
    int mat_indx;
    aabb bbox;

    // labels for selection
    std::unordered_set<std::string> labels = {"all"};
};

class translate : public hittable
{
public:
    translate(std::unique_ptr<hittable> _object, const vec3& _offset);

    bool hit(const ray& _r, interval _ray_t, hit_record& _rec) const override;

    aabb bounding_box() const override { return bbox; }

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


private:
    std::unique_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
};

#endif
