#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <raytracer/vec3.hpp>
#include <raytracer/ray.hpp>
#include <raytracer/utils.hpp>

namespace rt
{

struct CameraConfig 
{
    int image_width = 400;
    double aspect_ratio = 16.0 / 9.0;
    double vfov = 60.0;
    Point3 look_from = Point3(0, 0, 0);
    Point3 look_at = Point3(1, 0, 0);
    Vec3 v_up = Vec3(0, 1, 0);
};


class Camera
{
private:
    //initialized by struct
    double aspect_ratio_;
    int image_width_;
    Point3 camera_center_;
    Vec3 u_;
    Vec3 v_;
    Vec3 w_;

    //computed
    int image_height_;
    Point3 pixel_00_loc_;
    Vec3 pixel_delta_u_;
    Vec3 pixel_delta_v_;
    
public:
    
    int image_width()  const { return image_width_; }
    int image_height() const { return image_height_; }

    void initialize(const CameraConfig& config)
    {
        image_width_ = config.image_width;
        aspect_ratio_ = config.aspect_ratio;
        camera_center_ = config.look_from;

        image_height_ = static_cast<int>(image_width_ / aspect_ratio_);
        if (image_height_ < 1) image_height_ = 1;

        // viewport dims
        double theta = utils::degrees_to_radians(config.vfov);
        double h = std::tan(theta / 2.0);
        double focus_dist = (config.look_from - config.look_at).length();
        double viewport_height = 2.0 * h * focus_dist;
        double viewport_width = viewport_height * (static_cast<double>(image_width_) / image_height_);

        //ortonormal base
        w_ = (config.look_from - config.look_at).unit_vector();
        u_ = cross(config.v_up, w_).unit_vector();
        v_ = cross(w_, u_);

        // oriented vec on borders
        Vec3 viewport_u = viewport_width * u_;
        Vec3 viewport_v = viewport_height * -v_;

        //space between pixels
        pixel_delta_u_ = viewport_u / image_width_;
        pixel_delta_v_ = viewport_v / image_height_;

        //1st pixel up left
        Point3 viewport_upper_left = camera_center_ - (focus_dist * w_) - (viewport_u / 2.0) - (viewport_v / 2.0);
        pixel_00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
    }

    Ray get_ray(int x, int y) const
    {
        auto px = rt::utils::random_double(-0.5, 0.5);
        auto py = rt::utils::random_double(-0.5, 0.5);
        auto pixel_sample = pixel_00_loc_ + ((x + px) * pixel_delta_u_) + ((y + py) * pixel_delta_v_);
        return Ray(camera_center_, pixel_sample - camera_center_);
    }

};

}

#endif
