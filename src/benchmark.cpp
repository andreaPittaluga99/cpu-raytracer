#include <iostream>
#include <chrono>
#include <memory>
#include <omp.h>

#include <raytracer/camera.hpp>
#include <raytracer/renderer.hpp>
#include <raytracer/hittable_list.hpp>
#include <raytracer/sphere.hpp>
#include <raytracer/materials.hpp>

double run_render_pass(rt::Renderer& renderer, const rt::HittableList& world, const rt::Camera& camera, const rt::RenderConfig& config)
{
    auto start = std::chrono::steady_clock::now();
    [[maybe_unused]] auto buffer = renderer.render(world, camera, config);
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main()
{
    //scene setup
    rt::HittableList world;
    auto mat_ground = std::make_shared<rt::Lambertian>(rt::Color(0.8, 0.8, 0.0));
    auto mat_center = std::make_shared<rt::Lambertian>(rt::Color(0.1, 0.2, 0.5));
    auto mat_left   = std::make_shared<rt::Dielectric>(rt::Color(1.0, 1.0, 1.0), 1.5);
    auto mat_right  = std::make_shared<rt::Metallic>(rt::Color(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<rt::Sphere>(rt::Point3( 0.0, -100.5, -1.0), 100.0, mat_ground));
    world.add(std::make_shared<rt::Sphere>(rt::Point3( 0.0,    0.0, -1.0),   0.5, mat_center));
    world.add(std::make_shared<rt::Sphere>(rt::Point3(-1.0,    0.0, -1.0),   0.5, mat_left));
    world.add(std::make_shared<rt::Sphere>(rt::Point3( 1.0,    0.0, -1.0),   0.5, mat_right));

    //camera e renderer setup
    rt::CameraConfig cam_config;
    cam_config.image_width = 1920;
    cam_config.aspect_ratio = 16.0 / 9.0;
    cam_config.focal_length = 1.0;

    rt::Camera camera;
    camera.initialize(cam_config);

    rt::RenderConfig render_config;
    render_config.samples_per_pixel = 500;
    render_config.max_depth = 50;

    rt::Renderer renderer;

    int max_threads = omp_get_max_threads();

    std::cout << "Ray tracer benchmark\n";
    std::cout << "--------------------\n";
    std::cout << "resolution : " << camera.image_width() << "x" << camera.image_height() << "\n";
    std::cout << "max threads: " << max_threads << "\n\n";

    std::cout << "1 thread...  " << std::flush;
    omp_set_num_threads(1);

    //warm up
    run_render_pass(renderer, world, camera, render_config);

    double single_time = run_render_pass(renderer, world, camera, render_config);
    std::cout << single_time << " seconds\n";

    std::cout << max_threads << " threads... " << std::flush;
    omp_set_num_threads(max_threads);

    //warm up
    run_render_pass(renderer, world, camera, render_config);

    double multi_time = run_render_pass(renderer, world, camera, render_config);
    std::cout << multi_time << " seconds\n\n";

    double speedup = single_time / multi_time;
    std::cout << "results:\n";
    std::cout << "  speedup: " << speedup << "x\n";
    std::cout << "  efficiency: " << (speedup / max_threads) * 100.0 << "%\n";

    return 0;
}