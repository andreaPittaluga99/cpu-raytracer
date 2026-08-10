#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <raytracer/vec3.hpp>
#include <raytracer/ray.hpp>

namespace rt {

struct HitRecord 
{
    Point3 p;
    Vec3 normal;
    double t{0.0};
    bool front_face{true};

    // set the normal to always oppose the ray
    void set_face_normal(const Ray& r, const Vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable 
{
    public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const = 0;
};

} // namespace rt

#endif