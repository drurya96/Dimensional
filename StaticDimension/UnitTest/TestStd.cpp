#include "TestStd.h"

#include "StaticTime.h"
#include "StaticLength.h"
#include "StaticSpeed.h"

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
