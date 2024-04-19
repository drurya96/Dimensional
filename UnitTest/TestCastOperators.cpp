#include "TestCastOperators.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;
using namespace std;

TEST_F(CastOperatorTest, CastSpeed) {

   BaseDimension<tuple<LengthUnit<>>, tuple<TimeUnit<>>> testSpeed(10.0, vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds});
   BaseDimension<tuple<LengthUnit<>, TimeUnit<>>, tuple<>> testNotSpeed(10.0, vector<BaseUnit<>*>{&LengthUnits::Meters, & TimeUnits::Seconds}, vector<BaseUnit<>*>{});

   Speed mySpeed = testSpeed;

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // Speed notSpeed = testNotSpeed;
}

TEST_F(CastOperatorTest, CastLength) {

   BaseDimension<tuple<LengthUnit<>>, tuple<>> testLength(10.0, vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{});
   BaseDimension<tuple<TimeUnit<>>, tuple<>> testNotLength(10.0, vector<BaseUnit<>*>{&TimeUnits::Seconds}, vector<BaseUnit<>*>{});

   Length myLength = testLength;

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // Length notLength = testNotLength;
}

TEST_F(CastOperatorTest, CastTime) {

   BaseDimension<tuple<TimeUnit<>>, tuple<>> testTime(10.0, vector<BaseUnit<>*>{&TimeUnits::Seconds}, vector<BaseUnit<>*>{});
   BaseDimension<tuple<LengthUnit<>>, tuple<>> testNotTime(10.0, vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{});

   Time myTime = testTime;

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // Time notTime = testNotTime;
}
