#include "TestFundamentalConversions.h"

#include "TimeDimension.h"
#include "LengthDimension.h"

TEST_F(FundamentalConversions, TestTime) {
   Dimension::Time myTime = Dimension::Time(30.0, &Dimension::TimeUnits::Seconds);
   
   ASSERT_DOUBLE_EQ(myTime.GetVal(std::vector<Dimension::BaseUnit*>{&Dimension::TimeUnits::Seconds}, std::vector<Dimension::BaseUnit*>{}), 30.0);
   ASSERT_DOUBLE_EQ(myTime.GetVal(std::vector<Dimension::BaseUnit*>{&Dimension::TimeUnits::Minutes}, std::vector<Dimension::BaseUnit*>{}), 0.5);
}


TEST_F(FundamentalConversions, TestLength) {
   Dimension::Length myLength = Dimension::Length(10.0, &Dimension::LengthUnits::Meters);

   ASSERT_DOUBLE_EQ(myLength.GetVal(std::vector<Dimension::BaseUnit*>{&Dimension::LengthUnits::Meters}, std::vector<Dimension::BaseUnit*>{}), 10.0);
   ASSERT_DOUBLE_EQ(myLength.GetVal(std::vector<Dimension::BaseUnit*>{&Dimension::LengthUnits::Feet}, std::vector<Dimension::BaseUnit*>{}), 32.8084);
}

// Add more test cases as needed
