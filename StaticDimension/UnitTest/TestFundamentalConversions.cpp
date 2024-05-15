#include "TestFundamentalConversions.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
//#include "MassDimension.h"

TEST_F(FundamentalConversions, TestTime) {

   //Dimension::Time myBadTime = Dimension::Time<Dimension::Feet>(10.0); // Correctly fails to compile - Cannot create a Time object using Feet.

   Dimension::Time myTime = Dimension::Time(30.0);

   ASSERT_DOUBLE_EQ(myTime.GetTime<Dimension::Seconds>(), 30.0);
   ASSERT_DOUBLE_EQ(myTime.GetTime<Dimension::Minutes>(), 0.5);
   ASSERT_DOUBLE_EQ(myTime.GetTime<Dimension::Hours>(), 0.008333333333333333333);

   Dimension::Time myTime2 = Dimension::Time<Dimension::Minutes>(60.0);
   ASSERT_DOUBLE_EQ(myTime2.GetTime<Dimension::Hours>(), 1.0); // This is a test that conversions can pass through the primary unit
}


TEST_F(FundamentalConversions, TestLength) {
   Dimension::Length myLength = Dimension::Length(10.0);

   ASSERT_DOUBLE_EQ(myLength.GetLength<Dimension::Meters>(), 10.0);
   ASSERT_DOUBLE_EQ(myLength.GetLength<Dimension::Feet>(), 32.8084);
}

/*
TEST_F(FundamentalConversions, TestMass) {
   Dimension::Mass myMass = Dimension::Mass(1000.0, &Dimension::MassUnits::Gram());

   ASSERT_NEAR(myMass.GetVal(UnitList(Dimension::MassUnits::Pound()), UnitList()), 2.2046226218, 0.0001);
   ASSERT_NEAR(myMass.GetVal(UnitList(Dimension::MassUnits::Ounce()), UnitList()), 35.273962, 0.0001);

}
*/