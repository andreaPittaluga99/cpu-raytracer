#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <memory>

#include <raytracer/vec3.hpp>
#include <raytracer/ray.hpp>


namespace rt {

class Material;

struct HitRecord 
{
    Point3 p;
    Vec3 normal;
    double t{0.0};
    bool front_face{true};

    /*
    we use a raw pointer to get better performance with parallelization
    doing so, we don't have to deal with std::shared_ptr overhead for every ray
    material is still managed with shared_ptr by the objects in the scene
    */

    const Material* mat{nullptr};

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