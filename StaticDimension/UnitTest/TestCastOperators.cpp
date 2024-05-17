#include "TestCastOperators.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;
using namespace std;

TEST_F(CastOperatorTest, CastSpeed) {

   BaseDimension<tuple<Meters>, tuple<Seconds>> testSpeed(10.0);
   BaseDimension<tuple<Meters, Seconds>, tuple<>> testNotSpeed(10.0);

   Speed mySpeed = testSpeed;

   Speed<Feet, Minutes> differentSpeed = testSpeed;


   // TODO: I need to find a way to ensure the following DOES NOT compile
   // Speed notSpeed = testNotSpeed;
}

TEST_F(CastOperatorTest, CastLength) {

   BaseDimension<tuple<Meters>, tuple<>> testLength(10.0);
   BaseDimension<tuple<Seconds>, tuple<>> testNotLength(10.0);

   Length myLength = testLength;

   Length<Feet> differentLength = testLength;

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // Length notLength = testNotLength;
}

TEST_F(CastOperatorTest, CastTime) {

   BaseDimension<tuple<Seconds>, tuple<>> testTime(10.0);
   BaseDimension<tuple<Meters>, tuple<>> testNotTime(10.0);

   Time myTime = testTime;

   Time<Minutes> differentTime = testTime;

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // Time notTime = testNotTime;
}

