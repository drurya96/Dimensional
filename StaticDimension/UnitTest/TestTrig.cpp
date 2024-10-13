#include "TestTrig.h"

#include "StaticTime.h"
#include "StaticLength.h"
#include "StaticSpeed.h"
#include "StaticAngle.h"

#include <algorithm>

using namespace StaticDimension;
using StaticDimension::cos;
using StaticDimension::sin;
using StaticDimension::tan;
using std::numbers::pi;

// @todo Still need to verify angles outside range [0,2pi]

TEST_F(TrigTest, CosTest) {

   ASSERT_NEAR(cos(Angle<Radian>(pi * 2.0)), 1.0, TOLERANCE);
   ASSERT_NEAR(cos(Angle<Radian>(pi * 1.0)), -1.0, TOLERANCE);
   ASSERT_NEAR(cos(Angle<Radian>(pi / 2.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(cos(Angle<Radian>((3.0 * pi) / 2.0)), 0.0, TOLERANCE);

   ASSERT_NEAR(cos(Angle<Radian>( pi / 6.0)), (std::sqrt(3.0) / 2.0), TOLERANCE);
}

TEST_F(TrigTest, SinTest) {

   ASSERT_NEAR(sin(Angle<Radian>(pi * 2.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(sin(Angle<Radian>(pi * 1.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(sin(Angle<Radian>(pi / 2.0)), 1.0, TOLERANCE);
   ASSERT_NEAR(sin(Angle<Radian>((3.0 * pi) / 2.0)), -1.0, TOLERANCE);

   ASSERT_NEAR(sin(Angle<Radian>( pi / 6.0)), 0.5, TOLERANCE);
}

TEST_F(TrigTest, TanTest) {

   ASSERT_NEAR(tan(Angle<Radian>(pi * 0.0)), 0.0, TOLERANCE);
   ASSERT_NEAR(tan(Angle<Radian>(pi / 4.0)), 1.0, TOLERANCE);
   ASSERT_NEAR(tan(Angle<Radian>(3.0 * pi / 4.0)), -1.0, TOLERANCE);

   ASSERT_NEAR(tan(Angle<Radian>(pi / 6.0)), std::sqrt(3.0) / 3.0, TOLERANCE);
   ASSERT_NEAR(tan(Angle<Radian>(pi / 3.0)), std::sqrt(3.0), TOLERANCE);
}

TEST_F(TrigTest, AcosTest) {
   
   ASSERT_NEAR(StaticDimension::acos(1.0).GetAngle<Radian>(), 0.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::acos(0.0).GetAngle<Radian>(), pi / 2.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::acos(-1.0).GetAngle<Radian>(), pi, TOLERANCE);

   ASSERT_NEAR(StaticDimension::acos(std::sqrt(3.0) / 2.0).GetAngle<Radian>(), pi / 6.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::acos(0.5).GetAngle<Radian>(), pi / 3.0, TOLERANCE);
}

TEST_F(TrigTest, AsinTest) {

   ASSERT_NEAR(StaticDimension::asin(0.0).GetAngle<Radian>(), 0.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::asin(1.0).GetAngle<Radian>(), pi / 2.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::asin(-1.0).GetAngle<Radian>(), -pi / 2.0, TOLERANCE);

   ASSERT_NEAR(StaticDimension::asin(0.5).GetAngle<Radian>(), pi / 6.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::asin(std::sqrt(3.0) / 2.0).GetAngle<Radian>(), pi / 3.0, TOLERANCE);
}

TEST_F(TrigTest, AtanTest) {

   ASSERT_NEAR(StaticDimension::atan(0.0).GetAngle<Radian>(), 0.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::atan(1.0).GetAngle<Radian>(), pi / 4.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::atan(-1.0).GetAngle<Radian>(), -pi / 4.0, TOLERANCE);

   ASSERT_NEAR(StaticDimension::atan(std::sqrt(3.0)).GetAngle<Radian>(), pi / 3.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::atan(1.0 / std::sqrt(3.0)).GetAngle<Radian>(), pi / 6.0, TOLERANCE);
}

TEST_F(TrigTest, Atan2Test) {

   ASSERT_NEAR(StaticDimension::atan2(Length<Meters>(0.0), Length<Meters>(1.0)).GetAngle<Radian>(), 0.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::atan2(Length<Meters>(1.0), Length<Meters>(0.0)).GetAngle<Radian>(), pi / 2.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::atan2(Length<Meters>(-1.0), Length<Meters>(0.0)).GetAngle<Radian>(), -pi / 2.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::atan2(Length<Meters>(0.0), Length<Meters>(-1.0)).GetAngle<Radian>(), pi, TOLERANCE);
   ASSERT_NEAR(StaticDimension::atan2(Length<Meters>(1.0), Length<Meters>(1.0)).GetAngle<Radian>(), pi / 4.0, TOLERANCE);
   ASSERT_NEAR(StaticDimension::atan2(Length<Meters>(-1.0), Length<Meters>(-1.0)).GetAngle<Radian>(), -3.0 * pi / 4.0, TOLERANCE);

   // Ensure resolution of BaseDimension to named dimensions work
   ASSERT_NEAR(StaticDimension::atan2(Length<Meters>(0.0), BaseDimension<std::tuple<Meters>, std::tuple<>>(1.0)).GetAngle<Radian>(), 0.0, TOLERANCE);
}

TEST_F(TrigTest, HypotTest) {

   //auto ret = hypot(Length<Meters>(1.0), Length<Meters>(1.0));


   //ASSERT_NEAR(StaticDimension::hypot(Length<Meters>(0.0), Length<Meters>(1.0)).GetAngle<Radian>(), 0.0, TOLERANCE);

   using std::tuple;

   // @todo Add a way to call the short-hand getters from the appropriate BaseDimension without casting

   // Simple cases
   ASSERT_NEAR((hypot(Length<Meters>(3.0), Length<Meters>(4.0)).GetVal<tuple<Meters>, tuple<>>()), 5.0, TOLERANCE);
   ASSERT_NEAR((hypot(Length<Meters>(5.0), Length<Meters>(12.0)).GetVal<tuple<Meters>, tuple<>>()), 13.0, TOLERANCE);
   
   // Test when one side is zero
   ASSERT_NEAR((hypot(Length<Meters>(0.0), Length<Meters>(5.0)).GetVal<tuple<Meters>, tuple<>>()), 5.0, TOLERANCE);
   ASSERT_NEAR((hypot(Length<Meters>(7.0), Length<Meters>(0.0)).GetVal<tuple<Meters>, tuple<>>()), 7.0, TOLERANCE);

   // Testing small floating-point values
   ASSERT_NEAR((hypot(Length<Meters>(1e-10), Length<Meters>(1e-10)).GetVal<tuple<Meters>, tuple<>>()), std::sqrt(2) * 1e-10, TOLERANCE);

   // Testing larger values to verify stability
   ASSERT_NEAR((hypot(Length<Meters>(1e10), Length<Meters>(1e10)).GetVal<tuple<Meters>, tuple<>>()), std::sqrt(2.0) * 1e10, TOLERANCE);

   // Diagonal distance in a unit square (1-1 sqrt(2) triangle)
   ASSERT_NEAR((hypot(Length<Meters>(1.0), Length<Meters>(1.0)).GetVal<tuple<Meters>, tuple<>>()), std::sqrt(2.0), TOLERANCE);

   // Test negative values (hypot treats negative values as positive)
   ASSERT_NEAR((hypot(Length<Meters>(-3.0), Length<Meters>(4.0)).GetVal<tuple<Meters>, tuple<>>()), 5.0, TOLERANCE);
   ASSERT_NEAR((hypot(Length<Meters>(5.0), Length<Meters>(-12.0)).GetVal<tuple<Meters>, tuple<>>()), 13.0, TOLERANCE); 

   // Test for very large differences in magnitude
   ASSERT_NEAR((hypot(Length<Meters>(1e10), Length<Meters>(1.0)).GetVal<tuple<Meters>, tuple<>>()), 1e10, TOLERANCE);
   
}