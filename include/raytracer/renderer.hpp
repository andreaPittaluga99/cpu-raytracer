#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <limits>
#include <raytracer/color.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/hittable.hpp>
#include <raytracer/camera.hpp>
#include <raytracer/materials.hpp>

namespace rt
{
    
struct RenderConfig
{
    int samples_per_pixel = 100;
    int max_depth = 50;
};

class Renderer
{
private:
    rt::Color ray_color(const rt::Ray& r, const rt::Hittable& world, int depth) const
    {
        if (depth <= 0)
        {
            return rt::Color(0.0, 0.0, 0.0);
        }

        rt::HitRecord rec;
        //we use 0.001 instead of 0 to avoid surface accuracy issues
        if(world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec))
        {
            rt::Ray scattered;
            rt::Color attenuation;
            if (rec.mat && rec.mat->scatter (r, rec, attenuation, scattered))
            {
                return attenuation * ray_color(scattered, world, depth - 1);
            }

            //if the mat completly assorbs the light return full black
            return rt::Color(0.0, 0.0, 0.0);
        }

        // if the ray misses, we draw the bg
        rt::Vec3 unit_direction = r.direction().unit_vector();
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * rt::Color(1.0, 1.0, 1.0) + a * rt::Color(0.5, 0.7, 1.0);
    }

public:
    std::vector<Color> render(const Hittable& world, const Camera& camera, const RenderConfig& config) const
    {
        int width = camera.image_width();
        int height = camera.image_height();
        
        std::vector<Color> image_buffer(width * height);

        #pragma omp parallel for schedule(dynamic)
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Color pixel_color(0.0, 0.0, 0.0);

                for (int s = 0; s < config.samples_per_pixel; ++s)
                {
                    Ray r = camera.get_ray(x, y);
                    pixel_color += ray_color(r, world, config.max_depth);
                }

                //save mean in buffer
                image_buffer[y * width + x] = pixel_color / config.samples_per_pixel;
            }
        }
        return image_buffer;
    }

};

}

#endif