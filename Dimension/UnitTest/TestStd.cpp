#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"
#include "AngleDimension.h"

#include <algorithm>

using namespace Dimension;


TEST_F(DimensionTest, StdMinMaxClamp) {

   Speed<Meters, Seconds> mySpeed1(10.0); 
   Speed<Meters, Seconds> mySpeed2(20.0); 
   
   ASSERT_NEAR((std::min(mySpeed1, mySpeed2).GetSpeed<Meters, Seconds>()), 10.0, TOLERANCE);

   ASSERT_NEAR((std::max(mySpeed1, mySpeed2).GetSpeed<Meters, Seconds>()), 20.0, TOLERANCE);
   
   ASSERT_NEAR((std::clamp(mySpeed1, Speed<Meters, Seconds>(5.0), Speed<Meters, Seconds>(20.0)).GetSpeed<Meters, Seconds>()), 10.0, TOLERANCE);
   ASSERT_NEAR((std::clamp(mySpeed1, Speed<Meters, Seconds>(12.0), Speed<Meters, Seconds>(20.0)).GetSpeed<Meters, Seconds>()), 12.0, TOLERANCE);
   ASSERT_NEAR((std::clamp(mySpeed1, Speed<Meters, Seconds>(5.0), Speed<Meters, Seconds>(8.0)).GetSpeed<Meters, Seconds>()), 8.0, TOLERANCE);


   // Below does not work yet.
   // std::clamp requires the lo and hi values be the same type, but Speed<Feet, Seconds> is not the same type as Speed<Meters, Second>, even if these are convertible
   // Consider finding a way to make this work.
   //ASSERT_NEAR((std::clamp(mySpeed1, Speed<Feet, Seconds>(5.0), Speed<Meters, Seconds>(8.0)).GetSpeed<Meters, Seconds>()), 8.0, TOLERANCE);

}

TEST_F(DimensionTest, DimensionalAbs) {
   
   ASSERT_NEAR((abs(Speed<Meters, Seconds>(-10.0)).GetSpeed<Meters, Seconds>()), 10.0, TOLERANCE);
   ASSERT_NEAR((abs(Speed<Meters, Seconds>(10.0)).GetSpeed<Meters, Seconds>()), 10.0, TOLERANCE);
}

TEST_F(DimensionTest, DimensionalRound) {
   
   ASSERT_NEAR((floor(Speed<Meters, Seconds>(1.5)).GetSpeed<Meters, Seconds>()), 1.0, TOLERANCE);
   ASSERT_NEAR((ceil(Speed<Meters, Seconds>(1.5)).GetSpeed<Meters, Seconds>()), 2.0, TOLERANCE);
   ASSERT_NEAR((round(Speed<Meters, Seconds>(1.3)).GetSpeed<Meters, Seconds>()), 1.0, TOLERANCE);
   ASSERT_NEAR((round(Speed<Meters, Seconds>(1.8)).GetSpeed<Meters, Seconds>()), 2.0, TOLERANCE);
}

TEST_F(DimensionTest, DimensionalModf) {

   Speed<Meters, Seconds> value;
   Speed<Meters, Seconds> decSpeed;
   Speed<Meters, Seconds> intSpeed;

   value = Speed<Meters, Seconds>(1.5);
   decSpeed = modf(value);
   ASSERT_NEAR((decSpeed.GetSpeed<Meters, Seconds>()), 0.5, TOLERANCE);
   
   value = Speed<Meters, Seconds>(2.8);
   decSpeed = modf(value, &intSpeed);
   ASSERT_NEAR((decSpeed.GetSpeed<Meters, Seconds>()), 0.8, TOLERANCE);
   ASSERT_NEAR((intSpeed.GetSpeed<Meters, Seconds>()), 2.0, TOLERANCE);

}

TEST_F(DimensionTest, DimensionalFmod) {

   using Dimension::pi;

   ASSERT_NEAR((fmod(Time<Minutes>(130), Time<Minutes>(60)).GetTime<Minutes>()), 10.0, TOLERANCE);
   ASSERT_NEAR((fmod(Angle<Radians>(3*pi), Angle<Radians>(2*pi)).GetAngle<Radians>()), pi, TOLERANCE);
}
