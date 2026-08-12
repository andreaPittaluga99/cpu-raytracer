#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <cmath>

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
    
    Metallic(const rt::Color& albedo, double roughness)
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

class Dielectric : public Material
{
private:
    rt::Color albedo_;
    double index_of_refraction_;
public:
    Dielectric (const rt::Color& albedo, double index_of_refraction)
    :   albedo_(albedo), index_of_refraction_(index_of_refraction) {}
    
    bool scatter (const rt::Ray& r_in, const rt::HitRecord& rec, rt::Color& attenuation, rt::Ray& scattered) const override
    {
        //since dielectic mat doesn't absorbe light, attenuation in pure white
        attenuation = rt::Color (1.0, 1.0, 1.0);

        bool is_front_face = rec.front_face;

        //we suppose that the index of refraction of the other material is air, wich is 1.0
        double eta = is_front_face ? (1.0 / index_of_refraction_) : index_of_refraction_;
        Vec3 unit_normal = rec.normal;

        Vec3 unit_vector = r_in.direction().unit_vector();

        //checks if refraction is possibile 
        double cos_theta = std::min(-dot(unit_vector, unit_normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = eta * sin_theta > 1.0;
        Vec3 r_out;
        if(cannot_refract)
        {
            r_out = unit_vector - 2 * dot(unit_vector, unit_normal) * unit_normal;
        }
        else
        {
            Vec3 r_out_perp = eta * (unit_vector + cos_theta * unit_normal);
            Vec3 r_out_parallel = -std::sqrt(std::abs(1.0 - r_out_perp.length_squared())) * unit_normal;
    
            r_out = r_out_perp + r_out_parallel;    
        }
        
        scattered = rt::Ray(rec.p, r_out);
        return true;
    }
};


}

#endif