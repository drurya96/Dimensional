#include "TestFundamentalConversions.h"

#include "TimeDimension.h"
#include "LengthDimension.h"

using Dimension::UnitList;

TEST_F(FundamentalConversions, TestTime) {

   Dimension::Time myTime = Dimension::Time(30.0, &Dimension::TimeUnits::Seconds());
   
   ASSERT_DOUBLE_EQ(myTime.GetVal(UnitList(Dimension::TimeUnits::Seconds()), UnitList()), 30.0);
   ASSERT_DOUBLE_EQ(myTime.GetVal(UnitList(Dimension::TimeUnits::Minutes()), UnitList()), 0.5);
   ASSERT_DOUBLE_EQ(myTime.GetVal(UnitList(Dimension::TimeUnits::Hours()), UnitList()), 0.008333333333333333333);

   Dimension::Time myTime2 = Dimension::Time(60.0, &Dimension::TimeUnits::Minutes());
   ASSERT_DOUBLE_EQ(myTime2.GetVal(UnitList(Dimension::TimeUnits::Hours()), UnitList()), 1.0); // This is a test that conversions can pass through the primary unit
}


TEST_F(FundamentalConversions, TestLength) {
   Dimension::Length myLength = Dimension::Length(10.0, &Dimension::LengthUnits::Meters());

   ASSERT_DOUBLE_EQ(myLength.GetVal(UnitList(Dimension::LengthUnits::Meters()), UnitList()), 10.0);
   ASSERT_DOUBLE_EQ(myLength.GetVal(UnitList(Dimension::LengthUnits::Feet()), UnitList()), 32.8084);
}
