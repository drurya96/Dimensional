#include "DimensionTest.h"

using namespace dimension;

TEST(CompoundConversions, Testspeed) {

   using namespace std;

   speed<meters, seconds> myspeed(10.0);

   ASSERT_NEAR((get_speed_as<feet, seconds>(myspeed)), 32.8084, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, minutes>(myspeed)), 600.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<feet, minutes>(myspeed)), 1968.504, TOLERANCE);
}
