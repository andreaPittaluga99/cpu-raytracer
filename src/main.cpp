#include <iostream>

#include <memory>
#include <limits>

#include <raytracer/vec3.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/hittable_list.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/utils.hpp>
#include <raytracer/color.hpp>
#include <raytracer/materials.hpp>

rt::Color ray_color(const rt::Ray& r, const rt::Hittable& world, int depth) 
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

int main()
{
    constexpr double aspect_ratio = 16.0 / 9.0;
    constexpr int image_width = 400;
    constexpr int image_height = static_cast<int>(image_width / aspect_ratio);

    //cam and viewport
    constexpr double focal_length = 1.0;
    constexpr double viewport_height = 2.0;
    constexpr double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    constexpr rt::Point3 camera_center(0, 0, 0);

    constexpr rt::Vec3 viewport_u(viewport_width, 0, 0);
    constexpr rt::Vec3 viewport_v(0, -viewport_height, 0); 

    constexpr rt::Vec3 pixel_delta_u = viewport_u / image_width;
    constexpr rt::Vec3 pixel_delta_v = viewport_v / image_height;


    constexpr rt::Point3 viewport_upper_left = camera_center - rt::Vec3(0, 0, focal_length) - viewport_u / 2.0 - viewport_v / 2.0;
    
    constexpr rt::Point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    constexpr int samples_per_pixel = 300;

    //instantiate the world
    rt::HittableList world;
    //left sphere
    auto material_left = std::make_shared<rt::Lambertian>(rt::Color(0.8, 0.0, 0.0));
    world.add(std::make_shared<rt::Sphere>(rt::Point3(-0.6, 0.0, -1.0), 0.5, material_left));
    //right sphere
    auto material_right = std::make_shared<rt::Metallic>(rt::Color(0.8, 0.8, 0.8), 0.1);
    world.add(std::make_shared<rt::Sphere>(rt::Point3(0.6, 0.0, -1.0), 0.5, material_right));
    //bit spehere on the ground
    auto material_ground = std::make_shared<rt::Lambertian>(rt::Color(0.8, 0.8, 0.0));
    world.add(std::make_shared<rt::Sphere>(rt::Point3(0.0, -100.5, -1.0), 100.0, material_ground));

    int ray_depth = 100;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = 0; y < image_height; ++y) 
    {
        for (int x = 0; x < image_width; ++x) 
        {
            rt::Color pixel_color (0.0, 0.0, 0.0);
            for(int samples = 0; samples < samples_per_pixel; ++samples)
            {
                //random offset
                auto px = rt::utils::random_double(-0.5, 0.5);
                auto py = rt::utils::random_double(-0.5, 0.5);

                auto pixel_sample = pixel00_loc + ((x + px) * pixel_delta_u) + ((y + py) * pixel_delta_v);

                auto ray_direction = pixel_sample - camera_center;
                rt::Ray ray (camera_center, ray_direction);

                pixel_color += ray_color (ray, world, ray_depth);
            }
            //we get the mean of the colours computed
            pixel_color = pixel_color / samples_per_pixel;

            rt::write_color(std::cout, pixel_color);
        }
    }
    return 0;
}