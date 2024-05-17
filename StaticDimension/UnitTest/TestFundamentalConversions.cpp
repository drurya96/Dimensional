#include "TestFundamentalConversions.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"

TEST_F(FundamentalConversions, TestTime) {

   //Dimension::Time myBadTime = Dimension::Time<Dimension::Feet>(10.0); // Correctly fails to compile - Cannot create a Time object using Feet.

   Dimension::Time myTime = Dimension::Time(30.0);

   ASSERT_NEAR(myTime.GetTime<Dimension::Seconds>(), 30.0, TOLERANCE);
   ASSERT_NEAR(myTime.GetTime<Dimension::Minutes>(), 0.5, TOLERANCE);
   ASSERT_NEAR(myTime.GetTime<Dimension::Hours>(), 0.008333333333333333333, TOLERANCE);

   Dimension::Time myTime2 = Dimension::Time<Dimension::Minutes>(60.0);
   ASSERT_NEAR(myTime2.GetTime<Dimension::Hours>(), 1.0, TOLERANCE); // This is a test that conversions can pass through the primary unit
}


TEST_F(FundamentalConversions, TestLength) {
   Dimension::Length myLength = Dimension::Length(10.0);

   ASSERT_NEAR(myLength.GetLength<Dimension::Meters>(), 10.0, TOLERANCE);
   ASSERT_NEAR(myLength.GetLength<Dimension::Feet>(), 32.8084, TOLERANCE);
}


TEST_F(FundamentalConversions, TestMass) {
   Dimension::Mass<Dimension::Grams> myMass{1000.0};

   ASSERT_NEAR(myMass.GetMass<Dimension::Pounds>(), 2.2046226218, TOLERANCE);
   ASSERT_NEAR(myMass.GetMass<Dimension::Ounces>(), 35.273962, TOLERANCE);

}
