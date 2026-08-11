#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <raytracer/vec3.hpp>
#include <raytracer/hittable.hpp>
#include <raytracer/utils.hpp>

namespace rt
{

class Material
{
private:

public:
    Material() = default;
    virtual bool scatter (const rt::Ray& r_in, const rt::HitRecord& rec, rt::Color& attenuation, rt::Ray& scattered) const = 0;
};

class Lambertian : public Material
{
private:
    rt::Color albedo_;
public:
    Lambertian (const rt::Color& albedo)
    :   albedo_(albedo) {}

    bool scatter([[maybe_unused]]const rt::Ray& r_in, const rt::HitRecord& rec, rt::Color& attenuation, rt::Ray& scattered) const override
    {
        // generate new ray dispersion
        auto scatter_direction = rec.normal + rt::utils::random_unit_vector();

        scattered = rt::Ray(rec.p, scatter_direction);
        attenuation = albedo_;

        return true;
    }
    
};

class Metallic : public Material
{
private:
    rt::Color albedo_;
    double roughness_;
public:
    
    Metallic(rt::Color albedo, double roughness)
    :   albedo_(albedo), roughness_(roughness) 
    {
        if (roughness < 0.0 || roughness > 1.0)
        {
            throw std::invalid_argument("Material error: roughness must be between 0 and 1");
        }
    }

    bool scatter (const rt::Ray& r_in, const rt::HitRecord& rec, rt::Color& attenuation, rt::Ray& scattered) const override
    {
        attenuation = albedo_;

        Vec3 v = r_in.direction().unit_vector();
        Vec3 n = rec.normal;

        Vec3 r = v - 2 * dot(v, n) * n;

        scattered = rt::Ray(rec.p, r);

        return (dot(scattered.direction(), rec.normal) > 0);
    }

};

}

#endif