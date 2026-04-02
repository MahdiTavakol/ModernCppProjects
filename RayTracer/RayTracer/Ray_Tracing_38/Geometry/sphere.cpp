#include "sphere.h"

sphere::sphere(
    const point3& _center,
    double _radius,
    std::unique_ptr<material> _mat)
    : 
    hittable{"sphere", std::move(_mat)},
    center{_center, vec3(0, 0, 0)}, radius{std::fmax(0, _radius)}
{
    auto rvec = vec3(radius, radius, radius);
    bbox = aabb(_center - rvec, _center + rvec);
}

sphere::sphere(const point3& _center1,
               const point3& _center2,
               double _radius,
               std::unique_ptr<material> _mat)
    : 
    hittable{ "sphere",std::move(_mat)},
    center{ _center1, _center2 - _center1 }, radius{ std::fmax(0, _radius) }
{
    auto rvec = vec3(radius, radius, radius);
    aabb box1(center.at(0) - rvec, center.at(0) + rvec);
    aabb box2(center.at(1) - rvec, center.at(1) + rvec);
    bbox = aabb(box1, box2);
}

bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const {
    point3 current_center = center.at(r.time());
    vec3 oc = current_center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;

    auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - current_center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat = mat.get();

    return true;
}

bool sphere::compare(hittable* rhs_, const double& tol_) const
{
    auto rhs_cast = dynamic_cast<sphere*>(rhs_);
    if (!rhs_cast)
        throw std::invalid_argument("Different types!");


    ray dist = center - rhs_cast->center;
    point3 distOrig = dist.origin();
    vec3 distDir = dist.direction();


    double distTm = dist.time();
    double distOrigLength = distOrig.length();
    double distDirLength = distDir.length();
    double delta = radius - rhs_cast->radius;

    if (std::abs(distTm) >= tol_)
        return true;
    if (std::abs(distOrigLength) >= tol_)
        return true;
    if (std::abs(distDirLength) >= tol_)
        return true;
    if (std::abs(delta) >= tol_)
        return true;
    return false;
}

bool sphere::comparator(const std::unique_ptr<hittable>& rhs_) const
{
    auto rhs_cast = dynamic_cast<sphere*>(rhs_.get());
    if (!rhs_cast)
        throw std::invalid_argument("Different types!");

    if (radius < rhs_cast->radius)
        return true;
    return false;

}

void sphere::get_sphere_uv(const point3& p, double& u, double& v)
{

    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}
