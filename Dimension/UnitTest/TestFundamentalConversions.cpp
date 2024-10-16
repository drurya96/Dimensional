#include "TestFundamentalConversions.h"

#include "TimeDimension.h"
#include "Length.h"
#include "Mass.h"
#include "Angle.h"

using namespace Dimension;

TEST_F(FundamentalConversions, TestTime) {

   //Time myBadTime = Time<Feet>(10.0); // Correctly fails to compile - Cannot create a Time object using Feet.

   Time myTime = Time<Seconds>(30.0);

   ASSERT_NEAR(myTime.GetTime<Seconds>(), 30.0, TOLERANCE);
   ASSERT_NEAR(myTime.GetTime<Minutes>(), 0.5, TOLERANCE);
   ASSERT_NEAR(myTime.GetTime<Hours>(), 0.008333333333333333333, TOLERANCE);

   Time myTime2 = Time<Minutes>(60.0);
   ASSERT_NEAR(myTime2.GetTime<Hours>(), 1.0, TOLERANCE); // This is a test that conversions can pass through the primary unit
}


TEST_F(FundamentalConversions, TestLength) {
   Length myLength = Length<Meters>(10.0);

   ASSERT_NEAR(myLength.GetLength<Meters>(), 10.0, TOLERANCE);
   ASSERT_NEAR(myLength.GetLength<Feet>(), 32.8084, TOLERANCE);

   ASSERT_NEAR((GetSlope<Conversion<Meters, Feet>>()), 3.280839895, TOLERANCE);
   ASSERT_NEAR((GetOffset<Conversion<Meters, Feet>>()), 0.0, TOLERANCE);
   ASSERT_NEAR((GetSlope<Conversion<Meters, Meters>>()), 1.0, TOLERANCE);

   ASSERT_NEAR((Convert<Meters, Feet>(10.0)), 32.8084, TOLERANCE);
}


TEST_F(FundamentalConversions, TestMass) {
   Mass<Grams> myMass{1000.0};

   ASSERT_NEAR(myMass.GetMass<PoundMass>(), 2.2046226218, TOLERANCE);
   ASSERT_NEAR(myMass.GetMass<Ounces>(), 35.273962, TOLERANCE);


   ASSERT_NEAR(myMass.GetMass<KiloGrams>(), 1.0, TOLERANCE);
   ASSERT_NEAR(myMass.GetMass<MilliGrams>(), 1000000.0, TOLERANCE);
}

TEST_F(FundamentalConversions, TestAngle) {
   Angle<Radian> myAngle{3.0};

   ASSERT_NEAR(myAngle.GetAngle<Radian>(), 3.0, TOLERANCE);
   ASSERT_NEAR(myAngle.GetAngle<Degree>(), 171.887, TOLERANCE);
}
