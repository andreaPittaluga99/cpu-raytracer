#include <iostream>
#include <memory>
#include <vector>

#include <raytracer/camera.hpp>
#include <raytracer/renderer.hpp>
#include <raytracer/hittable_list.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/materials.hpp>
#include <raytracer/color.hpp>

int main()
{
    // world init
    rt::HittableList world;

    auto material_ground = std::make_shared<rt::Lambertian>(rt::Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<rt::Lambertian>(rt::Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<rt::Dielectric>(rt::Color(1.0, 1.0, 1.0), 1.5);
    auto material_right  = std::make_shared<rt::Metallic>(rt::Color(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<rt::Sphere>(rt::Point3( 0.0, -100.5, -5.0), 100.0, material_ground));
    world.add(std::make_shared<rt::Sphere>(rt::Point3( 0.0, 0.0, -5.0), 0.5, material_center));
    world.add(std::make_shared<rt::Sphere>(rt::Point3(-1.0, 0.0, -5.0), 0.5, material_left));
    world.add(std::make_shared<rt::Sphere>(rt::Point3( 1.0, 0.0, -5.0), 0.5, material_right));

    //camera init
    rt::CameraConfig cam_config;
    cam_config.image_width = 400;
    cam_config.aspect_ratio = 16.0 / 9.0;
    cam_config.focal_length = 2.0;
    cam_config.viewport_height = 1.2;

    rt::Camera camera;
    camera.initialize(cam_config);

    //renderer init
    rt::RenderConfig render_config;
    render_config.samples_per_pixel = 100;
    render_config.max_depth = 50;
    rt::Renderer renderer;

    std::vector<rt::Color> image_buffer = renderer.render(world, camera, render_config);

    // I/O
    int width = camera.image_width();
    int height = camera.image_height();

    std::cout << "P3\n" << width << ' ' << height << "\n255\n";
    for (const auto& pixel_color : image_buffer)
    {
        rt::write_color(std::cout, pixel_color);
    }

    return 0;
}