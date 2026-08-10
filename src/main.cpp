#include <iostream>

#include <memory>
#include <limits>

#include <raytracer/vec3.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/hittable_list.hpp>
#include <raytracer/sphere.hpp>

rt::Color ray_color(const rt::Ray& r, const rt::Hittable& world) 
{
    rt::HitRecord rec;
    //we use 0.001 instead of 0 to avoid surface accuracy issues
    if(world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec))
    {
        return 0.5 * (rec.normal + rt::Color(1.0, 1.0, 1.0));
    }

    // if the ray misses, we draw the bg
    rt::Vec3 unit_direction = r.direction().unit_vector();
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * rt::Color(1.0, 1.0, 1.0) + a * rt::Color(0.5, 0.7, 1.0);
}


void write_color(std::ostream& out, const rt::Color& pixel_color) {
    int rbyte = static_cast<int>(255.999 * pixel_color.x());
    int gbyte = static_cast<int>(255.999 * pixel_color.y());
    int bbyte = static_cast<int>(255.999 * pixel_color.z());

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

int main() {
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

    //instantiate the world
    rt::HittableList world;
    //center sphere
    world.add(std::make_shared<rt::Sphere>(rt::Point3(0.0, 0.0, -1.0), 0.5));
    //bit spehere on the ground
    world.add(std::make_shared<rt::Sphere>(rt::Point3(0.0, -100.5, -1.0), 100.0));

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int y = 0; y < image_height; ++y) 
    {
        for (int x = 0; x < image_width; ++x) 
        {
            auto pixel_center = pixel00_loc + (x * pixel_delta_u) + (y * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            rt::Ray r(camera_center, ray_direction);

            rt::Color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
    return 0;
}