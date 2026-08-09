#ifndef RAY_HPP
#define RAY_HPP

#include <raytracer/vec3.hpp>

namespace rt
{
class Ray
{
private:
    Point3 orig_{};
    Vec3 dir_{};
public:
    constexpr Ray() = default;
    constexpr Ray(const Point3& orig, const Vec3& dir)
    :   orig_ (orig),
        dir_(dir){}
    

    [[nodiscard]] constexpr Point3 origin() const noexcept { return orig_; }
    [[nodiscard]] constexpr Vec3 direction() const noexcept { return dir_; }
    
    [[nodiscard]] constexpr Point3 at(double t) const noexcept { return orig_ + (dir_ * t); }
    
};

}//namepace rt
#endif