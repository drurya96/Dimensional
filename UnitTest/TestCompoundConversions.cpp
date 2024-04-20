#include "TestCompoundConversions.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

TEST_F(CompoundConversions, TestSpeed) {

   using namespace Dimension;
   using namespace std;

   Speed mySpeed(10.0, &LengthUnits::Meters, &TimeUnits::Seconds);

   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit*>{&LengthUnits::Meters}, vector<BaseUnit*>{&TimeUnits::Seconds}), 10.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit*>{&LengthUnits::Feet}, vector<BaseUnit*>{&TimeUnits::Seconds}), 32.8084);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit*>{&LengthUnits::Meters}, vector<BaseUnit*>{&TimeUnits::Minutes}), 600.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit*>{&LengthUnits::Feet}, vector<BaseUnit*>{&TimeUnits::Minutes}), 1968.504);
}
