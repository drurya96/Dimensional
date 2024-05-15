#include "TestCompoundConversions.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;

TEST_F(CompoundConversions, TestSpeed) {

   using namespace std;

   Speed<Meters, Seconds> mySpeed(10.0);

   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Seconds>()), 32.8084);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Meters, Minutes>()), 600.0);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Minutes>()), 1968.504);
}
