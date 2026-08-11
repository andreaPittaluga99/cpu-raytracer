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

}

#endif