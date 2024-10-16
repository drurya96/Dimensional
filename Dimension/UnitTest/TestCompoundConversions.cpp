#include "TestCompoundConversions.h"

#include "TimeDimension.h"
#include "Length.h"
#include "Speed.h"

using namespace Dimension;

TEST_F(CompoundConversions, TestSpeed) {

   using namespace std;

   Speed<Meters, Seconds> mySpeed(10.0);

   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Seconds>()), 32.8084, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Meters, Minutes>()), 600.0, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Minutes>()), 1968.504, TOLERANCE);
}
