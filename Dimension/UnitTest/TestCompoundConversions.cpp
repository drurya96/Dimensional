#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;

TEST(CompoundConversions, TestSpeed) {

   using namespace std;

   Speed<Meters, Seconds> mySpeed(10.0);

   ASSERT_NEAR((getSpeed<Feet, Seconds>(mySpeed)), 32.8084, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Minutes>(mySpeed)), 600.0, TOLERANCE);
   ASSERT_NEAR((getSpeed<Feet, Minutes>(mySpeed)), 1968.504, TOLERANCE);
}
