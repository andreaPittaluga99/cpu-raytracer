#ifndef VEC3_HPP
#define VEC3_HPP

#include <array>
#include <type_traits>
#include <concepts>
#include <stdexcept>
#include <functional>
#include <cmath>
#include <iostream>
#include <ostream>

namespace rt{
class Vec3
{
    public:

    /******************************** Constructors *************************************/
    constexpr Vec3() = default;

    constexpr Vec3(double x, double y, double z) 
        : coord_{x, y, z} 
    {}

    template <typename T>
    requires (std::is_arithmetic_v<T>)
    constexpr explicit Vec3(const std::array<T, 3>& coord)
    : coord_{static_cast<double>(coord[0]), static_cast<double>(coord[1]), static_cast<double>(coord[2])}
    {}

    ~Vec3() = default;

    /***************************** Getters ************************/
    constexpr double x () const noexcept {return coord_[0];}
    constexpr double y () const noexcept {return coord_[1];}
    constexpr double z () const noexcept {return coord_[2];}

    constexpr double r () const noexcept {return coord_[0];}
    constexpr double g () const noexcept {return coord_[1];}
    constexpr double b () const noexcept {return coord_[2];}

    /***************************** Operators overloading ************************/
    constexpr Vec3 operator-() const {
        Vec3 ret (-coord_[0], -coord_[1], -coord_[2]); 
        return ret;
    }

    constexpr const double& operator[] (size_t i) const 
    {
        if (i > 2)
        {
            throw std::out_of_range("index out of range");
        }
        return coord_[i];
    }

    constexpr double& operator[] (size_t i) 
    {
        if (i > 2)
        {
            throw std::out_of_range("index out of range");
        }
        return coord_[i];
    }   

    constexpr Vec3& operator+=(const Vec3& other) {
        return elementwise_assign(other, std::plus<>{});
    }

    /************************ Op *******************************/
    
    constexpr Vec3& operator-=(const Vec3& other) {
        return elementwise_assign(other, std::minus<>{});
    }

    constexpr Vec3& operator*=(const Vec3& other) {
        return elementwise_assign(other, std::multiplies<>{});
    }

    constexpr Vec3& operator/=(const Vec3& other) {
        return elementwise_assign(other, std::divides<>{});
    }

    [[nodiscard]] constexpr Vec3 operator+ (const Vec3& other) const {
        Vec3 res(*this);
        res += other;
        return res; 
    }

    [[nodiscard]] constexpr Vec3 operator- (const Vec3& other) const {
        Vec3 res(*this);
        res -= other;
        return res; 
    }

    [[nodiscard]] constexpr Vec3 operator* (const Vec3& other) const {
        Vec3 res(*this);
        res *= other;
        return res; 
    }

    [[nodiscard]] constexpr Vec3 operator/ (const Vec3& other) const {
        Vec3 res(*this);
        res /= other;
        return res; 
    }


    //scalars

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    constexpr Vec3& operator+=(Scalar value){
        return scalar_assign(value, std::plus<>{});
    }

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    constexpr Vec3& operator-=(Scalar value){
        return scalar_assign(value, std::minus<>{});
    }

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    constexpr Vec3& operator*=(Scalar value){
        return scalar_assign(value, std::multiplies<>{});
    }
    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    constexpr Vec3& operator/=(Scalar value){
        return scalar_assign(value, std::divides<>{});
    }

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    [[nodiscard]] constexpr Vec3 operator+(Scalar value) const 
    {
        Vec3 res(*this);
        res += value;
        return res;
    }

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    [[nodiscard]] constexpr Vec3 operator-(Scalar value) const 
    {
        Vec3 res(*this);
        res -= value;
        return res;
    }

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    [[nodiscard]] constexpr Vec3 operator*(Scalar value) const 
    {
        Vec3 res(*this);
        res *= value;
        return res;
    }

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    [[nodiscard]] constexpr Vec3 operator/(Scalar value) const 
    {
        Vec3 res(*this);
        res /= value;
        return res;
    }
    
    /***************************** Functions **********************************/
    [[nodiscard]] constexpr double length_squared () const noexcept { return (coord_[0] * coord_[0]) + (coord_[1] * coord_[1]) + (coord_[2] * coord_[2]); }

    [[nodiscard]] double length() const { return std::sqrt(length_squared()); }

    [[nodiscard]] Vec3 unit_vector() const { return *this / length();}


    /*****************************************************************************/
    /****************************** private **************************************/
    /*****************************************************************************/
    private:
    std::array<double, 3> coord_ {0.0, 0.0, 0.0};


    /************************** Helpers **************************************/
    template<typename Scalar, typename Op>
    constexpr Vec3& scalar_assign(Scalar value, Op op) 
    {
        for (auto& x : coord_) 
        {
            x = op(x, value);
        }

        return *this;
    }

    template<typename Op>
    constexpr Vec3& elementwise_assign(const Vec3& other, Op op) 
    {
        for (size_t i = 0; i < coord_.size(); ++i) 
        {
            coord_[i] = op(coord_[i], other.coord_[i]);
        }
    
        return *this;
    }

};//class

    using Point3 = Vec3;
    using Color = Vec3;

    [[nodiscard]] constexpr double dot (const Vec3& a, const Vec3& b)
    {
        return (a.x() * b.x()) + (a.y() * b.y()) + (a.z() * b.z());
    }

    [[nodiscard]] constexpr Vec3 cross(const Vec3& u, const Vec3& v) 
    {
        return Vec3(
            u.y() * v.z() - u.z() * v.y(),
            u.z() * v.x() - u.x() * v.z(),
            u.x() * v.y() - u.y() * v.x()
            );
    }

    /************** Free op ********************/

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    [[nodiscard]] constexpr Vec3 operator+(Scalar value, const Vec3& v) 
    {
        return v + value;
    }

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    [[nodiscard]] constexpr Vec3 operator*(Scalar value, const Vec3& v) 
    {
        return v * value;
    }

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    [[nodiscard]] constexpr Vec3 operator-(Scalar value, const Vec3& v) 
    {
        return Vec3(value - v.x(), value - v.y(), value - v.z());
    }

    template<typename Scalar>
    requires (std::is_arithmetic_v<Scalar>)
    [[nodiscard]] constexpr Vec3 operator/(Scalar value, const Vec3& v) 
    {
        return Vec3(value / v.x(), value / v.y(), value / v.z());
    }


    inline std::ostream& operator<<(std::ostream& out, const Vec3& v) 
    {
        return out << v.x() << ' ' << v.y() << ' ' << v.z();
    }
}//namespace rt

#endif