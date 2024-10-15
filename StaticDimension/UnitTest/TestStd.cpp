#include "TestStd.h"

#include "StaticTime.h"
#include "StaticLength.h"
#include "StaticSpeed.h"
#include "StaticAngle.h"

#include <algorithm>

using namespace StaticDimension;


TEST_F(StdTest, StdMinMaxClamp) {

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

TEST_F(StdTest, DimensionalAbs) {

   using std::tuple;
   
   ASSERT_NEAR((abs(Speed<Meters, Seconds>(-10.0)).GetVal<tuple<Meters>, tuple<Seconds>>()), 10.0, TOLERANCE);
   ASSERT_NEAR((abs(Speed<Meters, Seconds>(10.0)).GetVal<tuple<Meters>, tuple<Seconds>>()), 10.0, TOLERANCE);
}

TEST_F(StdTest, DimensionalRound) {

   using std::tuple;
   
   ASSERT_NEAR((floor(Speed<Meters, Seconds>(1.5)).GetVal<tuple<Meters>, tuple<Seconds>>()), 1.0, TOLERANCE);
   ASSERT_NEAR((ceil(Speed<Meters, Seconds>(1.5)).GetVal<tuple<Meters>, tuple<Seconds>>()), 2.0, TOLERANCE);
   ASSERT_NEAR((round(Speed<Meters, Seconds>(1.3)).GetVal<tuple<Meters>, tuple<Seconds>>()), 1.0, TOLERANCE);
   ASSERT_NEAR((round(Speed<Meters, Seconds>(1.8)).GetVal<tuple<Meters>, tuple<Seconds>>()), 2.0, TOLERANCE);
}

TEST_F(StdTest, DimensionalModf) {

   using std::tuple;
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

TEST_F(StdTest, DimensionalFmod) {
   using std::tuple;
   using std::numbers::pi;

   ASSERT_NEAR((fmod(Time<Minutes>(130), Time<Minutes>(60)).GetVal<tuple<Minutes>, tuple<>>()), 10.0, TOLERANCE);
   ASSERT_NEAR((fmod(Angle<Radian>(3*pi), Angle<Radian>(2*pi)).GetVal<tuple<Radian>, tuple<>>()), pi, TOLERANCE);
}
