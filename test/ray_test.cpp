#include <gtest/gtest.h>
#include <raytracer/ray.hpp>

TEST(RayTest, DefaultConstructor) {
    rt::Ray r;
    
    EXPECT_DOUBLE_EQ(r.origin().x(), 0.0);
    EXPECT_DOUBLE_EQ(r.origin().y(), 0.0);
    EXPECT_DOUBLE_EQ(r.origin().z(), 0.0);

    EXPECT_DOUBLE_EQ(r.direction().x(), 0.0);
    EXPECT_DOUBLE_EQ(r.direction().y(), 0.0);
    EXPECT_DOUBLE_EQ(r.direction().z(), 0.0);
}

TEST(RayTest, ParametrizedConstructorAndGetters) {
    rt::Point3 origin(1.0, 2.0, 3.0);
    rt::Vec3 direction(4.0, 5.0, 6.0);
    rt::Ray r(origin, direction);

    EXPECT_DOUBLE_EQ(r.origin().x(), 1.0);
    EXPECT_DOUBLE_EQ(r.origin().y(), 2.0);
    EXPECT_DOUBLE_EQ(r.origin().z(), 3.0);

    EXPECT_DOUBLE_EQ(r.direction().x(), 4.0);
    EXPECT_DOUBLE_EQ(r.direction().y(), 5.0);
    EXPECT_DOUBLE_EQ(r.direction().z(), 6.0);
}

TEST(RayTest, PointAtParameter) {
    rt::Point3 origin(2.0, 3.0, 4.0);
    rt::Vec3 direction(1.0, 0.0, -1.0);
    rt::Ray r(origin, direction);

    auto p_zero = r.at(0.0);
    EXPECT_DOUBLE_EQ(p_zero.x(), 2.0);
    EXPECT_DOUBLE_EQ(p_zero.y(), 3.0);
    EXPECT_DOUBLE_EQ(p_zero.z(), 4.0);
    auto p_pos = r.at(2.0);
    EXPECT_DOUBLE_EQ(p_pos.x(), 4.0);
    EXPECT_DOUBLE_EQ(p_pos.y(), 3.0);
    EXPECT_DOUBLE_EQ(p_pos.z(), 2.0);
    auto p_neg = r.at(-1.0);
    EXPECT_DOUBLE_EQ(p_neg.x(), 1.0);
    EXPECT_DOUBLE_EQ(p_neg.y(), 3.0);
    EXPECT_DOUBLE_EQ(p_neg.z(), 5.0);
}

//we use static_assert to enforce compile time evaluation
//because google's EXPECT_ works at runtime
//if ray or at() are not constexpr, the uild would fail here
TEST(RayTest, ConstexprEvaluation) {
    constexpr rt::Point3 origin(1.0, 1.0, 1.0);
    constexpr rt::Vec3 direction(0.0, 1.0, 0.0);
    constexpr rt::Ray r(origin, direction);
    constexpr auto p = r.at(5.0);

    static_assert(p.x() == 1.0, "Constexpr evaluation failed on X");
    static_assert(p.y() == 6.0, "Constexpr evaluation failed on Y");
    static_assert(p.z() == 1.0, "Constexpr evaluation failed on Z");
}