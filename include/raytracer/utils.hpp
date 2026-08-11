#ifndef UTILS_HPP
#define UTILS_HPP

#include <random>
#include <cmath>

#include <raytracer/ray.hpp>

namespace rt::utils
{
    inline double random_double()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        //by default is already [0.0, 1.0)
        std::uniform_real_distribution<double> distrib(0.0, 1.0);
        return distrib(gen);
    }

    inline double random_double(double min, double max)
    {
        return min + (max - min) * random_double();
    }

    inline Vec3 random_vec3() 
    {
        return Vec3(random_double(), random_double(), random_double());
    }

    inline Vec3 random_vec3(double min, double max)
    {
        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    inline Vec3 random_unit_vector()
    {
        while (true) 
        {
            auto p = random_vec3(-1.0, 1.0);
            auto lensq = p.lenght_squared();
            // so we dont divide for 0
            if (1e-160 < lensq && lensq <= 1.0)
            {
                return p / std::sqrt(lensq);
            }
        }
    }
}

#endif