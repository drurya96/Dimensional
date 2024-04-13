#include "TestFundamentalConversions.h"


#include "TimeDimension.h"
#include "LengthDimension.h"

TEST_F(FundamentalConversions, TestTime) {
   Dimension::Time myTime = Dimension::Time(30.0, &Dimension::TimeUnits::Seconds);
   
   ASSERT_DOUBLE_EQ(myTime.GetVal(&Dimension::TimeUnits::Seconds), 30.0);
   ASSERT_DOUBLE_EQ(myTime.GetVal(&Dimension::TimeUnits::Minutes), 0.5);
}


TEST_F(FundamentalConversions, TestLength) {
   Dimension::Length myLength = Dimension::Length(10.0, &Dimension::LengthUnits::Meters);

   ASSERT_DOUBLE_EQ(myLength.GetVal(&Dimension::LengthUnits::Meters), 10.0);
   ASSERT_DOUBLE_EQ(myLength.GetVal(&Dimension::LengthUnits::Feet), 32.8084);
}

// Add more test cases as needed
