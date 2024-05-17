#include "TestSimplification.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;
using namespace std;

TEST_F(SimplificationTest, SimplificationWithMath) {

   // TODO: Consider adding a .Simplify() method to BaseDimension
   Length<Meters> length1(10);
   Length<Feet> length2(20);

   Time<Seconds> time1(5);
   Time<Minutes> time2(12);

   // This is 1.0416666667 Meters-Seconds-Seconds/Feet-Minutes
   // Simplification occurs during division, so this resolves to just Seconds.
   // However, Seconds in the numerator cancels Minutes in the denominator and Meters in the numerator cancels Feet in the denominator
   // So the value should be 0.056959025955088914139 Seconds
   auto test = ((length1 * time1) / (length2 * time2)) * time1;
   ASSERT_TRUE((is_same_v<decltype(test), BaseDimension<tuple<Seconds>, tuple<>>>));
   
   ASSERT_NEAR((test.GetVal<std::tuple<Seconds>, std::tuple<>>()), 0.056959027777777775, TOLERANCE); // TODO: Double check precision
}

TEST_F(SimplificationTest, Simplification) {

   Length<Meters> length1(10);
   Length<Feet> length2(20);

   Time<Seconds> time1(5);
   Time<Minutes> time2(12);

   // This is 10.0 (Meters-Mintues / Feet-Seconds-Minutes-Minutes)
   auto test_before = BaseDimension<std::tuple<Meters, Minutes>, std::tuple<Seconds, Feet, Minutes>>(10.0);
   ASSERT_TRUE((is_same_v<decltype(test_before), BaseDimension<tuple<Meters, Minutes>, tuple<Seconds, Feet, Minutes>>>));


   // After simplifying, Meters/Feet cancel and Minutes/Seconds cancel
   // Note, simplification currently works left-to-right, so Minutes won't cancel with Minutes in this example

   // This means the final type will be a per-minutes
   auto test_after = Simplify(test_before);

   ASSERT_TRUE((is_same_v<decltype(test_after), BaseDimension<tuple<>, tuple<Minutes>>>));
   ASSERT_NEAR((test_after.GetVal<std::tuple<>, std::tuple<Minutes>>()), 1968.504, TOLERANCE);
}

// TODO: Validate simplification resulting in unitless
// TODO: How to handle unitless?