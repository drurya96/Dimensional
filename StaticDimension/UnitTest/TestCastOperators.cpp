#include "TestCastOperators.h"

#include "StaticTime.h"
#include "StaticLength.h"
#include "StaticSpeed.h"

using namespace StaticDimension;
using namespace std;

TEST_F(CastOperatorTest, CastSpeed) {

   BaseDimension<tuple<Meters>, tuple<Seconds>> testSpeed(10.0);
   BaseDimension<tuple<Meters, Seconds>, tuple<>> testNotSpeed(10.0);

   Speed<Meters, Seconds> mySpeed1 = testSpeed;
   Speed mySpeed2 = testSpeed; // Uses template deduction guide

   Speed<Feet, Minutes> differentSpeed = testSpeed;


   // TODO: I need to find a way to ensure the following DOES NOT compile
   // Speed notSpeed = testNotSpeed;
}

TEST_F(CastOperatorTest, CastLength) {

   BaseDimension<tuple<Meters>, tuple<>> testLength(10.0);
   BaseDimension<tuple<Seconds>, tuple<>> testNotLength(10.0);

   Length<Meters> myLength1 = testLength;
   Length myLength2 = testLength; // Uses template deduction guide

   Length<Feet> differentLength = testLength;

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // Length notLength = testNotLength;
}

TEST_F(CastOperatorTest, CastTime) {

   BaseDimension<tuple<Seconds>, tuple<>> testTime(10.0);
   BaseDimension<tuple<Meters>, tuple<>> testNotTime(10.0);

   Time<Seconds> myTime1 = testTime;
   Time myTime2 = testTime; // Uses template deduction guide

   Time<Minutes> differentTime = testTime;

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // Time notTime = testNotTime;
}

TEST_F(CastOperatorTest, CastScalar) {

   BaseDimension<tuple<Seconds>, tuple<>> testTime(10.0);

   BaseDimension<tuple<>, tuple<>> scalar = testTime / Time<Seconds>(5.0);

   ASSERT_NEAR(scalar, 2.0, TOLERANCE);
}
