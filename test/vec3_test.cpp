#include <gtest/gtest.h>
#include <raytracer/vec3.hpp>

using namespace rt;

TEST(Vec3Test, ConstructionAndGetters) {
    Vec3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.x(), 1.0);
    EXPECT_DOUBLE_EQ(v.y(), 2.0);
    EXPECT_DOUBLE_EQ(v.z(), 3.0);
    EXPECT_DOUBLE_EQ(v.r(), 1.0);
    EXPECT_DOUBLE_EQ(v.g(), 2.0);
    EXPECT_DOUBLE_EQ(v.b(), 3.0);
}

TEST(Vec3Test, ArithmeticOperations) {
    Vec3 v1(1.0, 2.0, 3.0);
    Vec3 v2(4.0, 5.0, 6.0);

    Vec3 sum = v1 + v2;
    EXPECT_DOUBLE_EQ(sum.x(), 5.0);
    EXPECT_DOUBLE_EQ(sum.y(), 7.0);
    EXPECT_DOUBLE_EQ(sum.z(), 9.0);

    Vec3 scaled = 2.0 * v1;
    EXPECT_DOUBLE_EQ(scaled.x(), 2.0);
    EXPECT_DOUBLE_EQ(scaled.y(), 4.0);
    EXPECT_DOUBLE_EQ(scaled.z(), 6.0);
}

TEST(Vec3Test, LengthAndNormalization) {
    Vec3 v(3.0, 4.0, 0.0);
    EXPECT_DOUBLE_EQ(v.lenght_squared(), 25.0);
    EXPECT_DOUBLE_EQ(v.lenght(), 5.0);

    Vec3 u = v.unit_vector();
    EXPECT_NEAR(u.x(), 0.6, 1e-6);
    EXPECT_NEAR(u.y(), 0.8, 1e-6);
    EXPECT_NEAR(u.z(), 0.0, 1e-6);
    EXPECT_NEAR(u.lenght(), 1.0, 1e-6);
}

TEST(Vec3Test, VectorProducts) {
    EXPECT_DOUBLE_EQ(dot(Vec3(1, 0, 0), Vec3(0, 1, 0)), 0.0);
    EXPECT_DOUBLE_EQ(dot(Vec3(1, 2, 3), Vec3(4, 5, 6)), 32.0);

    Vec3 z = cross(Vec3(1, 0, 0), Vec3(0, 1, 0));
    EXPECT_DOUBLE_EQ(z.x(), 0.0);
    EXPECT_DOUBLE_EQ(z.y(), 0.0);
    EXPECT_DOUBLE_EQ(z.z(), 1.0);
}