#include "DimensionTest.h"

#include <algorithm>

using namespace dimension;
using dimension::cos;
using dimension::sin;
using dimension::tan;
using std::numbers::pi;

// @todo Still need to verify angles outside range [0,2pi]

TEST(TrigFunctions, CosTest) {

   ASSERT_NEAR(cos(angle<radians>(pi * 2.0)), 1.0, TOLERANCE);
   ASSERT_NEAR(cos(angle<radians>(pi * 1.0)), -1.0, TOLERANCE);
   ASSERT_NEAR(cos(angle<radians>(pi / 2.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(cos(angle<radians>((3.0 * pi) / 2.0)), 0.0, TOLERANCE);

   ASSERT_NEAR(cos(angle<radians>( pi / 6.0)), (std::sqrt(3.0) / 2.0), TOLERANCE);
}

TEST(TrigFunctions, SinTest) {

   ASSERT_NEAR(sin(angle<radians>(pi * 2.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(sin(angle<radians>(pi * 1.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(sin(angle<radians>(pi / 2.0)), 1.0, TOLERANCE);
   ASSERT_NEAR(sin(angle<radians>((3.0 * pi) / 2.0)), -1.0, TOLERANCE);

   ASSERT_NEAR(sin(angle<radians>( pi / 6.0)), 0.5, TOLERANCE);
}

TEST(TrigFunctions, TanTest) {

   ASSERT_NEAR(tan(angle<radians>(pi * 0.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(tan(angle<radians>(pi / 4.0)), 1.0, TOLERANCE);
   ASSERT_NEAR(tan(angle<radians>(3.0 * pi / 4.0)), -1.0, TOLERANCE);

   ASSERT_NEAR(tan(angle<radians>(pi / 6.0)), std::sqrt(3.0) / 3.0, TOLERANCE);
   ASSERT_NEAR(tan(angle<radians>(pi / 3.0)), std::sqrt(3.0), TOLERANCE);
}

TEST(TrigFunctions, AcosTest) {
   
   ASSERT_NEAR(get_angle_as<radians>(dimension::acos(1.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::acos(0.0)), pi / 2.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::acos(-1.0)), pi, TOLERANCE);

   ASSERT_NEAR(get_angle_as<radians>(dimension::acos(std::sqrt(3.0) / 2.0)), pi / 6.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::acos(0.5)), pi / 3.0, TOLERANCE);
}

TEST(TrigFunctions, AsinTest) {

   ASSERT_NEAR(get_angle_as<radians>(dimension::asin(0.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::asin(1.0)), pi / 2.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::asin(-1.0)), -pi / 2.0, TOLERANCE);

   ASSERT_NEAR(get_angle_as<radians>(dimension::asin(0.5)), pi / 6.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::asin(std::sqrt(3.0) / 2.0)), pi / 3.0, TOLERANCE);
}

TEST(TrigFunctions, AtanTest) {

   ASSERT_NEAR(get_angle_as<radians>(dimension::atan(0.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::atan(1.0)), pi / 4.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::atan(-1.0)), -pi / 4.0, TOLERANCE);

   ASSERT_NEAR(get_angle_as<radians>(dimension::atan(std::sqrt(3.0))), pi / 3.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::atan(1.0 / std::sqrt(3.0))), pi / 6.0, TOLERANCE);
}

TEST(TrigFunctions, Atan2Test) {

   ASSERT_NEAR(get_angle_as<radians>(dimension::atan2(length<meters>(0.0), length<meters>(1.0))), 0.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::atan2(length<meters>(1.0), length<meters>(0.0))), pi / 2.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::atan2(length<meters>(-1.0), length<meters>(0.0))), -pi / 2.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::atan2(length<meters>(0.0), length<meters>(-1.0))), pi, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::atan2(length<meters>(1.0), length<meters>(1.0))), pi / 4.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>(dimension::atan2(length<meters>(-1.0), length<meters>(-1.0))), -3.0 * pi / 4.0, TOLERANCE);

   // Ensure resolution of base_dimension to named dimensions work
   ASSERT_NEAR(get_angle_as<radians>(dimension::atan2(length<meters>(0.0), base_dimension<unit_exponent<meters>>(1.0))), 0.0, TOLERANCE);
}

TEST(TrigFunctions, HypotTest) {

   // Simple cases
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(3.0), length<meters>(4.0)))), 5.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(5.0), length<meters>(12.0)))), 13.0, TOLERANCE);
   
   // Test when one side is zero
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(0.0), length<meters>(5.0)))), 5.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(7.0), length<meters>(0.0)))), 7.0, TOLERANCE);

   // Testing small floating-point values
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(1e-10), length<meters>(1e-10)))), std::sqrt(2) * 1e-10, TOLERANCE);

   // Testing larger values to verify stability
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(1e10), length<meters>(1e10)))), std::sqrt(2.0) * 1e10, TOLERANCE);

   // Diagonal distance in a unit square (1-1 sqrt(2) triangle)
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(1.0), length<meters>(1.0)))), std::sqrt(2.0), TOLERANCE);

   // Test negative values (hypot treats negative values as positive)
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(-3.0), length<meters>(4.0)))), 5.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(5.0), length<meters>(-12.0)))), 13.0, TOLERANCE); 

   // Test for very large differences in magnitude
   ASSERT_NEAR(get_length_as<meters>((hypot(length<meters>(1e10), length<meters>(1.0)))), 1e10, TOLERANCE);
   
}
