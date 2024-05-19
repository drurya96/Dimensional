#include "TestCompoundConversions.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;

TEST_F(CompoundConversions, TestSpeed) {

   using namespace std;

   Speed mySpeed(10.0, &LengthUnits::Meters(), &TimeUnits::Seconds());

   ASSERT_DOUBLE_EQ(mySpeed.GetVal(UnitList(LengthUnits::Feet()),   UnitList(TimeUnits::Seconds())), 32.8084);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(UnitList(LengthUnits::Meters()), UnitList(TimeUnits::Minutes())), 600.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(UnitList(LengthUnits::Feet()),   UnitList(TimeUnits::Minutes())), 1968.504);
}
