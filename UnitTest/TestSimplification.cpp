#include "TestSimplification.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;
using namespace std;

TEST_F(SimplificationTest, SimplificationWithMath) {

   // TODO: Consider adding a .Simplify() method to BaseDimension
   Length length1 = Length(10, &LengthUnits::Meters);
   Length length2 = Length(20, &LengthUnits::Feet);

   Time time1 = Time(5, &TimeUnits::Seconds);
   Time time2 = Time(12, &TimeUnits::Minutes);

   // This is 1.0416666667 Meters-Seconds-Seconds/Feet-Minutes
   // Simplification occurs during division, so this resolves to just Seconds.
   // However, Seconds in the numerator cancels Minutes in the denominator and Meters in the numerator cancels Feet in the denominator
   // So the value should be 0.056959025955088914139 Seconds
   auto test = ((length1 * time1) / (length2 * time2)) * time1;
   ASSERT_TRUE((is_same<decltype(test), BaseDimension<tuple<TimeUnit<>>, tuple<>>>::value));
   
   ASSERT_DOUBLE_EQ(test.GetVal(vector<BaseUnit<>*>{&TimeUnits::Seconds}, vector<BaseUnit<>*>{}), 0.056959027777777775); // TODO: Double check precision
}

TEST_F(SimplificationTest, Simplification) {

   Length length1 = Length(10, &LengthUnits::Meters);
   Length length2 = Length(20, &LengthUnits::Feet);

   Time time1 = Time(5, &TimeUnits::Seconds);
   Time time2 = Time(12, &TimeUnits::Minutes);

   using namespace LengthUnits;
   using namespace TimeUnits;

   // This is 10.0 (Meters-Mintues / Feet-Seconds-Minutes-Minutes)
   auto test_before = BaseDimension<tuple<LengthUnit<>, TimeUnit<>>, tuple<TimeUnit<>, LengthUnit<>, TimeUnit<>>>(10.0, vector<BaseUnit<>*>{&Meters, &Minutes}, vector<BaseUnit<>*>{&Feet, &Seconds, &Minutes});
   ASSERT_TRUE((is_same<decltype(test_before), BaseDimension<tuple<LengthUnit<>, TimeUnit<>>, tuple<TimeUnit<>, LengthUnit<>, TimeUnit<>>>>::value));


   // After simplifying, Meters/Feet cancel and Minutes/Seconds cancel
   // Note, simplification currently works left-to-right, so Minutes won't cancel with Minutes in this example

   // This means the final type will be a per-minutes
   auto test_after = SimplifyBaseDimension(test_before);
   ASSERT_TRUE((is_same<decltype(test_after), BaseDimension<tuple<>, tuple<TimeUnit<>>>>::value));
   ASSERT_DOUBLE_EQ(test_after.GetVal(vector<BaseUnit<>*>{}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 1968.504);
}

// TODO: Validate simplification resulting in unitless
// TODO: How to handle unitless?