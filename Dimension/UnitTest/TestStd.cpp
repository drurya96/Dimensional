#include "DimensionTest.h"

#include <algorithm>

using namespace dimension;


TEST(StandardFunctions, StdMinMaxClamp) {

   speed<meters, seconds> myspeed1(10.0); 
   speed<meters, seconds> myspeed2(20.0); 
   
   ASSERT_NEAR((get_speed_as<meters, seconds>(std::min(myspeed1, myspeed2))), 10.0, TOLERANCE);

   ASSERT_NEAR((get_speed_as<meters, seconds>(std::max(myspeed1, myspeed2))), 20.0, TOLERANCE);
   
   ASSERT_NEAR((get_speed_as<meters, seconds>(std::clamp(myspeed1, speed<meters, seconds>(5.0), speed<meters, seconds>(20.0)))), 10.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, seconds>(std::clamp(myspeed1, speed<meters, seconds>(12.0), speed<meters, seconds>(20.0)))), 12.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, seconds>(std::clamp(myspeed1, speed<meters, seconds>(5.0), speed<meters, seconds>(8.0)))), 8.0, TOLERANCE);
   

   // Below does not work yet.
   // std::clamp requires the lo and hi values be the same type, but speed<Feet, seconds> is not the same type as speed<meters, Second>, even if these are convertible
   // Consider finding a way to make this work.
   // ASSERT_NEAR((get_speed_as<meters, seconds>(std::clamp(myspeed1, speed<Feet, seconds>(5.0), speed<meters, seconds>(8.0)))), 8.0, TOLERANCE);

}

TEST(StandardFunctions, DimensionalAbs) {
   
   ASSERT_NEAR((get_speed_as<meters, seconds>(abs(speed<meters, seconds>(-10.0)))), 10.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, seconds>(abs(speed<meters, seconds>(10.0)))), 10.0, TOLERANCE);
}

TEST(StandardFunctions, DimensionalRound) {
   
   ASSERT_NEAR((get_speed_as<meters, seconds>(floor(speed<meters, seconds>(1.5)))), 1.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, seconds>(ceil(speed<meters, seconds>(1.5)))), 2.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, seconds>(round(speed<meters, seconds>(1.3)))), 1.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, seconds>(round(speed<meters, seconds>(1.8)))), 2.0, TOLERANCE);
}

TEST(StandardFunctions, DimensionalModf) {

   speed<meters, seconds> value;
   speed<meters, seconds> decspeed;
   speed<meters, seconds> intspeed;

   value = speed<meters, seconds>(1.5);
   decspeed = modf(value);
   ASSERT_NEAR((get_speed_as<meters, seconds>(decspeed)), 0.5, TOLERANCE);
   
   value = speed<meters, seconds>(2.8);
   decspeed = modf(value, &intspeed);
   ASSERT_NEAR((get_speed_as<meters, seconds>(decspeed)), 0.8, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, seconds>(intspeed)), 2.0, TOLERANCE);

}

TEST(StandardFunctions, DimensionalFmod) {

   using std::numbers::pi;

   ASSERT_NEAR((get_timespan_as<minutes>(fmod(timespan<minutes>(130), timespan<minutes>(60)))), 10.0, TOLERANCE);
   ASSERT_NEAR(get_angle_as<radians>((fmod(angle<radians>(3*pi), angle<radians>(2*pi)))), pi, TOLERANCE);
}
