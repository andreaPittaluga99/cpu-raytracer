#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>

#include <raytracer/hittable.hpp>
#include <raytracer/vec3.hpp>
#include <raytracer/materials.hpp>

namespace rt
{
class Sphere : public Hittable
{
private:
    Point3 center_;
    double radius_;
    std::shared_ptr<Material> mat_;
public:
    Sphere (const Point3& center, const double radius, const std::shared_ptr<Material> mat)
    :   center_(center), radius_(radius), mat_(mat) {}

    bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override
    {
        rt::Vec3 oc = r.origin() - center_;

        double a = r.direction().length_squared();
        // h = b/2
        double h = rt::dot(r.direction(), oc);
        double c = oc.length_squared() - (radius_ * radius_);

        double discriminant_reduced = (h * h) - (a * c);

        if (discriminant_reduced < 0)
        {
            return false;
        }
        
        double sqrt_discriminant = std::sqrt(discriminant_reduced);

        double t = (-h - sqrt_discriminant) / a;
        if (ray_tmin < t && t < ray_tmax)
        {
            rec.t = t;
            rec.p = r.at(rec.t);
            auto normal = (rec.p - center_) / radius_;
            rec.set_face_normal(r, normal);
            rec.mat = mat_;
            return true;
        }
        //if out of range, we calculate the positive delta instead
        t = (-h + sqrt_discriminant) / a;
        if (ray_tmin < t && t < ray_tmax)
        {
            rec.t = t;
            rec.p = r.at(rec.t);
            auto normal = (rec.p - center_) / radius_;
            rec.set_face_normal(r, normal);
            rec.mat = mat_;
            return true;
        }

        // if is again out of range, we don't hit the ray doesn't hit the sphere
        return false;
    }
};

}

#endif