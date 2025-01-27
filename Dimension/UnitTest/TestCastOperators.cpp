#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;
using namespace std;

TEST(CastOperatorTests, CastSpeed) {

   BaseDimension<tuple<Meters>, tuple<Seconds>> testSpeed(10.0);

   Speed<Meters, Seconds> mySpeed1 = testSpeed;
   Speed mySpeed2 = testSpeed; // Uses template deduction guide
   Speed<Feet, Minutes> mySpeed3 = testSpeed; // Cast to different unit

   ASSERT_NEAR((getSpeed<Meters, Seconds>(mySpeed1)), 10.0, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(mySpeed2)), 10.0, TOLERANCE);
   ASSERT_NEAR((getSpeed<Meters, Seconds>(mySpeed3)), 10.0, TOLERANCE);

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // BaseDimension<tuple<Meters, Seconds>, tuple<>> testNotSpeed(10.0);
   // Speed notSpeed = testNotSpeed;
}

TEST(CastOperatorTests, CastLength) {

   BaseDimension<tuple<Meters>, tuple<>> testLength(10.0);

   Length<Meters> myLength1 = testLength;
   Length myLength2 = testLength; // Uses template deduction guide
   Length<Feet> myLength3 = testLength; // Cast to different unit

   ASSERT_NEAR(getLength<Meters>(myLength1), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<Meters>(myLength2), 10.0, TOLERANCE);
   ASSERT_NEAR(getLength<Meters>(myLength3), 10.0, TOLERANCE);

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // BaseDimension<tuple<Seconds>, tuple<>> testNotLength(10.0);
   // Length notLength = testNotLength;
}

TEST(CastOperatorTests, CastTime) {

   BaseDimension<tuple<Seconds>, tuple<>> testTime(10.0);

   Time<Seconds> myTime1 = testTime;
   Time myTime2 = testTime; // Uses template deduction guide
   Time<Minutes> myTime3 = testTime; // Cast to different unit

   ASSERT_NEAR(getTime<Seconds>(myTime1), 10.0, TOLERANCE);
   ASSERT_NEAR(getTime<Seconds>(myTime2), 10.0, TOLERANCE);
   ASSERT_NEAR(getTime<Seconds>(myTime3), 10.0, TOLERANCE);

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // BaseDimension<tuple<Meters>, tuple<>> testNotTime(10.0);
   // Time notTime = testNotTime;
}

TEST(CastOperatorTests, CastScalar) {

   BaseDimension<tuple<Seconds>, tuple<>> testTime(10.0);

   BaseDimension<tuple<>, tuple<>> scalar = testTime / Time<Seconds>(5.0);

   ASSERT_NEAR(scalar, 2.0, TOLERANCE);
}
