#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"
#include "EnergyDimension.h"

using namespace Dimension;
using namespace std;

TEST(Simplification, SimplificationWithMath) {

   Length<Meters> length1(10.0);
   Length<Feet> length2(20.0);

   Time<Seconds> time1(5.0);
   Time<Minutes> time2(12.0);

   // This is 1.0416666667 Meters-Seconds-Seconds/Feet-Minutes
   // Simplification occurs during division, so this resolves to just Seconds.
   // However, Seconds in the numerator cancels Minutes in the denominator and Meters in the numerator cancels Feet in the denominator
   // So the value should be 0.056959025955088914139 Seconds
   auto test = ((length1 * time1) / (length2 * time2)) * time1;
   //Time t = ((length1 * time1) / (length2 * time2)) * time1;
   ASSERT_TRUE((is_same_v<decltype(FullSimplify(test)), BaseDimension<UnitExponent<Seconds>>>));
   
   ASSERT_NEAR((get_dimension_as<UnitExponent<Seconds>>(test)), 0.056959027777777775, TOLERANCE); // TODO: Double check precision
   ASSERT_NEAR((get_time_as<Seconds>(test)), 0.056959027777777775, TOLERANCE); // TODO: Double check precision
}

TEST(Simplification, Simplification) {

   // This is 10.0 (Meters-Mintues / Feet-Seconds-Minutes-Minutes)
   auto test_before = BaseDimension<UnitExponent<Meters>, UnitExponent<Minutes>, UnitExponent<Seconds, -1>, UnitExponent<Feet, -1>, UnitExponent<Minutes, -1>>(10.0);
   ASSERT_TRUE((is_same_v<decltype(test_before), BaseDimension<UnitExponent<Meters>, UnitExponent<Minutes>, UnitExponent<Seconds, -1>, UnitExponent<Feet, -1>, UnitExponent<Minutes, -1>>>));


   // After simplifying, Meters/Feet cancel and Minutes/Seconds cancel
   // Note, simplification currently works left-to-right, so Minutes won't cancel with Minutes in this example

   // This means the final type will be a per-minutes
   auto test_after = FullSimplify(test_before);

   ASSERT_TRUE((is_same_v<decltype(test_after), BaseDimension<UnitExponent<Seconds, -1>>>));
   ASSERT_NEAR((get_dimension_as<UnitExponent<Minutes, -1>>(test_after)), 1968.504, TOLERANCE);
}


TEST(Simplification, SimplificationSpeedTypes)
{
   Speed<Meters, Seconds> speed(1);
   Time<Minutes> time(1);

   Length length = speed * time;
   //Length<> l = speed * time;
   //Length length = l;


   ASSERT_NEAR(get_length_as<Meters>(length), 60.0, TOLERANCE);

}

// TODO: Validate simplification resulting in unitless
// TODO: How to handle unitless?

TEST(Simplification, Validate_constexpr_behavior) {

    using namespace Dimension;
    using std::tuple;
    using std::cout;
    using std::endl;

    constexpr BaseDimension<UnitExponent<Meters>> obj1(10.0);
    constexpr BaseDimension<UnitExponent<Feet>> obj2(5.0);

    static_assert(get_dimension_as<UnitExponent<Meters>>(obj1) == 10.0, "Fail");
    static_assert(get_length_as<Meters>(obj1) == 10.0, "Fail");
    static_assert(obj1 > obj2, "Fail");
    static_assert(obj2 < obj1, "Fail");

    constexpr auto obj3 = obj1 + obj2;
    constexpr auto obj4 = obj1 - obj2;
    constexpr auto obj5 = obj1 * obj2;
    constexpr auto obj6 = obj1 / obj2;


    static_assert(obj3 > obj1, "Fail");
    static_assert(obj1 > obj4, "Fail");
    
    static_assert(get_dimension_as<UnitExponent<Meters, 2>>(obj5) > 10.0, "Fail");
    static_assert(get_dimension_as<>(obj6) > 0.0, "Fail");
    static_assert(obj6 > 0.0, "Fail");

    constexpr Length<Meters> obj7(15.0);
    static_assert(obj7 > obj1, "Fail");

    constexpr Length obj8 = obj1;
    static_assert(obj8 == obj1, "Fail");

    constexpr Speed<Meters, Seconds> speed1(10.0);
    static_assert(get_dimension_as<UnitExponent<Meters>, UnitExponent<Seconds, -1>>(speed1) == 10.0, "Fail");

    constexpr Energy<Joules> energy1(5.0);
    constexpr Energy<KiloGrams, Meters, Seconds> energy2(5.0);

    static_assert(energy1 == energy2, "Fail");
   
}

