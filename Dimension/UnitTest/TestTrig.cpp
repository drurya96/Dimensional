#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"
#include "AngleDimension.h"

#include <algorithm>

using namespace Dimension;
using Dimension::cos;
using Dimension::sin;
using Dimension::tan;
using Dimension::pi;

// @todo Still need to verify angles outside range [0,2pi]

TEST_F(DimensionTest, CosTest) {

   ASSERT_NEAR(cos(Angle<Radians>(pi * 2.0)), 1.0, TOLERANCE);
   ASSERT_NEAR(cos(Angle<Radians>(pi * 1.0)), -1.0, TOLERANCE);
   ASSERT_NEAR(cos(Angle<Radians>(pi / 2.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(cos(Angle<Radians>((3.0 * pi) / 2.0)), 0.0, TOLERANCE);

   ASSERT_NEAR(cos(Angle<Radians>( pi / 6.0)), (std::sqrt(3.0) / 2.0), TOLERANCE);
}

TEST_F(DimensionTest, SinTest) {

   ASSERT_NEAR(sin(Angle<Radians>(pi * 2.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(sin(Angle<Radians>(pi * 1.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(sin(Angle<Radians>(pi / 2.0)), 1.0, TOLERANCE);
   ASSERT_NEAR(sin(Angle<Radians>((3.0 * pi) / 2.0)), -1.0, TOLERANCE);

   ASSERT_NEAR(sin(Angle<Radians>( pi / 6.0)), 0.5, TOLERANCE);
}

TEST_F(DimensionTest, TanTest) {

   ASSERT_NEAR(tan(Angle<Radians>(pi * 0.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(tan(Angle<Radians>(pi / 4.0)), 1.0, TOLERANCE);
   ASSERT_NEAR(tan(Angle<Radians>(3.0 * pi / 4.0)), -1.0, TOLERANCE);

   ASSERT_NEAR(tan(Angle<Radians>(pi / 6.0)), std::sqrt(3.0) / 3.0, TOLERANCE);
   ASSERT_NEAR(tan(Angle<Radians>(pi / 3.0)), std::sqrt(3.0), TOLERANCE);
}

TEST_F(DimensionTest, AcosTest) {
   
   ASSERT_NEAR(getAngle<Radians>(Dimension::acos(1.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::acos(0.0)), pi / 2.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::acos(-1.0)), pi, TOLERANCE);

   ASSERT_NEAR(getAngle<Radians>(Dimension::acos(std::sqrt(3.0) / 2.0)), pi / 6.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::acos(0.5)), pi / 3.0, TOLERANCE);
}

TEST_F(DimensionTest, AsinTest) {

   ASSERT_NEAR(getAngle<Radians>(Dimension::asin(0.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::asin(1.0)), pi / 2.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::asin(-1.0)), -pi / 2.0, TOLERANCE);

   ASSERT_NEAR(getAngle<Radians>(Dimension::asin(0.5)), pi / 6.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::asin(std::sqrt(3.0) / 2.0)), pi / 3.0, TOLERANCE);
}

TEST_F(DimensionTest, AtanTest) {

   ASSERT_NEAR(getAngle<Radians>(Dimension::atan(0.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::atan(1.0)), pi / 4.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::atan(-1.0)), -pi / 4.0, TOLERANCE);

   ASSERT_NEAR(getAngle<Radians>(Dimension::atan(std::sqrt(3.0))), pi / 3.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::atan(1.0 / std::sqrt(3.0))), pi / 6.0, TOLERANCE);
}

TEST_F(DimensionTest, Atan2Test) {

   ASSERT_NEAR(getAngle<Radians>(Dimension::atan2(Length<Meters>(0.0), Length<Meters>(1.0))), 0.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::atan2(Length<Meters>(1.0), Length<Meters>(0.0))), pi / 2.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::atan2(Length<Meters>(-1.0), Length<Meters>(0.0))), -pi / 2.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::atan2(Length<Meters>(0.0), Length<Meters>(-1.0))), pi, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::atan2(Length<Meters>(1.0), Length<Meters>(1.0))), pi / 4.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>(Dimension::atan2(Length<Meters>(-1.0), Length<Meters>(-1.0))), -3.0 * pi / 4.0, TOLERANCE);

   // Ensure resolution of BaseDimension to named dimensions work
   ASSERT_NEAR(getAngle<Radians>(Dimension::atan2(Length<Meters>(0.0), BaseDimension<std::tuple<Meters>, std::tuple<>>(1.0))), 0.0, TOLERANCE);
}

TEST_F(DimensionTest, HypotTest) {

   // Simple cases
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(3.0), Length<Meters>(4.0)))), 5.0, TOLERANCE);
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(5.0), Length<Meters>(12.0)))), 13.0, TOLERANCE);
   
   // Test when one side is zero
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(0.0), Length<Meters>(5.0)))), 5.0, TOLERANCE);
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(7.0), Length<Meters>(0.0)))), 7.0, TOLERANCE);

   // Testing small floating-point values
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(1e-10), Length<Meters>(1e-10)))), std::sqrt(2) * 1e-10, TOLERANCE);

   // Testing larger values to verify stability
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(1e10), Length<Meters>(1e10)))), std::sqrt(2.0) * 1e10, TOLERANCE);

   // Diagonal distance in a unit square (1-1 sqrt(2) triangle)
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(1.0), Length<Meters>(1.0)))), std::sqrt(2.0), TOLERANCE);

   // Test negative values (hypot treats negative values as positive)
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(-3.0), Length<Meters>(4.0)))), 5.0, TOLERANCE);
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(5.0), Length<Meters>(-12.0)))), 13.0, TOLERANCE); 

   // Test for very large differences in magnitude
   ASSERT_NEAR(getLength<Meters>((hypot(Length<Meters>(1e10), Length<Meters>(1.0)))), 1e10, TOLERANCE);
   
}