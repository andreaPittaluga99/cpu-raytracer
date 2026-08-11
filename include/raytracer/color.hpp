#ifndef COLOR_HPP
#define COLOR_HPP

#include <cmath>
#include <ostream>
#include <algorithm>

#include <raytracer/vec3.hpp>
#include <raytracer/hittable.hpp>


namespace rt
{
    inline double linear_to_gamma_2(double linear_component)
    {
        return (linear_component > 0.0) ? std::sqrt(linear_component) : 0.0;
    }

    void write_color(std::ostream& out, const rt::Color& pixel_color) 
    {
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();

        r = linear_to_gamma_2(r);
        g = linear_to_gamma_2(g);
        b = linear_to_gamma_2(b);

        int rbyte = static_cast<int>(256 * std::clamp(r, 0.0, 0.999));
        int gbyte = static_cast<int>(256 * std::clamp(g, 0.0, 0.999));
        int bbyte = static_cast<int>(256 * std::clamp(b, 0.0, 0.999));

        out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
    }
}

#endif