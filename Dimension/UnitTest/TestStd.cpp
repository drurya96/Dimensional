#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"
#include "AngleDimension.h"

#include <algorithm>

using namespace Dimension;


TEST(StandardFunctions, StdMinMaxClamp) {

   Speed<Meters, Seconds> mySpeed1(10.0); 
   Speed<Meters, Seconds> mySpeed2(20.0); 
   
   ASSERT_NEAR((getSpeed<Meters, Seconds>(std::min(mySpeed1, mySpeed2))), 10.0, TOLERANCE);

   ASSERT_NEAR((getSpeed<Meters, Seconds>(std::max(mySpeed1, mySpeed2))), 20.0, TOLERANCE);
   
   ASSERT_NEAR((getSpeed<Meters, Seconds>(std::clamp(mySpeed1, Speed<Meters, Seconds>(5.0), Speed<Meters, Seconds>(20.0)))), 10.0, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(std::clamp(mySpeed1, Speed<Meters, Seconds>(12.0), Speed<Meters, Seconds>(20.0)))), 12.0, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(std::clamp(mySpeed1, Speed<Meters, Seconds>(5.0), Speed<Meters, Seconds>(8.0)))), 8.0, TOLERANCE);
   

   // Below does not work yet.
   // std::clamp requires the lo and hi values be the same type, but Speed<Feet, Seconds> is not the same type as Speed<Meters, Second>, even if these are convertible
   // Consider finding a way to make this work.
   // ASSERT_NEAR((getSpeed<Meters, Seconds>(std::clamp(mySpeed1, Speed<Feet, Seconds>(5.0), Speed<Meters, Seconds>(8.0)))), 8.0, TOLERANCE);

}

TEST(StandardFunctions, DimensionalAbs) {
   
   ASSERT_NEAR((getSpeed<Meters, Seconds>(abs(Speed<Meters, Seconds>(-10.0)))), 10.0, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(abs(Speed<Meters, Seconds>(10.0)))), 10.0, TOLERANCE);
}

TEST(StandardFunctions, DimensionalRound) {
   
   ASSERT_NEAR((getSpeed<Meters, Seconds>(floor(Speed<Meters, Seconds>(1.5)))), 1.0, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(ceil(Speed<Meters, Seconds>(1.5)))), 2.0, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(round(Speed<Meters, Seconds>(1.3)))), 1.0, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(round(Speed<Meters, Seconds>(1.8)))), 2.0, TOLERANCE);
}

TEST(StandardFunctions, DimensionalModf) {

   Speed<Meters, Seconds> value;
   Speed<Meters, Seconds> decSpeed;
   Speed<Meters, Seconds> intSpeed;

   value = Speed<Meters, Seconds>(1.5);
   decSpeed = modf(value);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(decSpeed)), 0.5, TOLERANCE);
   
   value = Speed<Meters, Seconds>(2.8);
   decSpeed = modf(value, &intSpeed);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(decSpeed)), 0.8, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(intSpeed)), 2.0, TOLERANCE);

}

TEST(StandardFunctions, DimensionalFmod) {

   using std::numbers::pi;

   ASSERT_NEAR((getTime<Minutes>(fmod(Time<Minutes>(130), Time<Minutes>(60)))), 10.0, TOLERANCE);
   ASSERT_NEAR(getAngle<Radians>((fmod(Angle<Radians>(3*pi), Angle<Radians>(2*pi)))), pi, TOLERANCE);
}
