#include "DimensionTest.h"

using namespace dimension;
using namespace std;

/*
TEST(Simplification, SimplificationWithMath) {

   length<meters> length1(10.0);
   length<feet> length2(20.0);

   timespan<seconds> time1(5.0);
   timespan<minutes> time2(12.0);

   // This is 1.0416666667 meters-seconds-seconds/feet-minutes
   // Simplification occurs during division, so this resolves to just seconds.
   // However, seconds in the numerator cancels minutes in the denominator and meters in the numerator cancels feet in the denominator
   // So the value should be 0.056959025955088914139 seconds
   auto test = ((length1 * time1) / (length2 * time2)) * time1;
   //timespan t = ((length1 * time1) / (length2 * time2)) * time1;
   ASSERT_TRUE((is_same_v<decltype(full_simplify(test)), base_dimension<unit_exponent<seconds>>>));
   
   ASSERT_NEAR((get_dimension_as<unit_exponent<seconds>>(test)), 0.056959027777777775, TOLERANCE); // TODO: Double check precision
   ASSERT_NEAR((get_timespan_as<seconds>(test)), 0.056959027777777775, TOLERANCE); // TODO: Double check precision
}
*/
/*
TEST(Simplification, Simplification) {

   // This is 10.0 (meters-Mintues / feet-seconds-minutes-minutes)
   auto test_before = base_dimension<unit_exponent<meters>, unit_exponent<minutes>, unit_exponent<seconds, -1>, unit_exponent<feet, -1>, unit_exponent<minutes, -1>>(10.0);
   ASSERT_TRUE((is_same_v<decltype(test_before), base_dimension<unit_exponent<meters>, unit_exponent<minutes>, unit_exponent<seconds, -1>, unit_exponent<feet, -1>, unit_exponent<minutes, -1>>>));


   // Effectively 32.8084 s-1


   // After simplifying, meters/feet cancel and minutes/minutes cancel

   // This means the final type will be a per-seconds
   auto test_after = full_simplify(test_before);

   ASSERT_TRUE((is_same_v<decltype(test_after)::units, std::tuple<unit_exponent<seconds, -1>>>));
   ASSERT_TRUE((is_same_v<decltype(test_after)::ratio, std::ratio<381, 1250>>)); // Feet to meters conversion - WRONG!!! THIS NEEDS TO BE METERS TO FEET!!! <---- THIS IS THE IMMEDIATE PROBLEM!
   ASSERT_TRUE((is_same_v<decltype(test_after), base_dimension<unit_exponent<seconds, -1>, std::ratio<381,1250>>>));
   ASSERT_NEAR((get_dimension_as<unit_exponent<minutes, -1>>(test_after)), 1968.504, TOLERANCE); // Currently yielding 600... something is wrong. The above issue but also the ratio doesn't seem to be applied.. so 2 completely distinct problems
}
*/

// Helper to print a single ratio_exponent
template<typename RE>
void print_ratio_exponent()
{
    std::cout
        << "ratio: " << RE::ratio::num << "/" << RE::ratio::den
        << ", exponent: " << RE::exponent::num << "/" << RE::exponent::den << "\n";
}

// Print all ratio_exponents in a tuple
template<typename... REs>
void print_tuple(const std::tuple<REs...>&)
{
    (print_ratio_exponent<REs>(), ...); // Fold expression
}

TEST(Simplification, SimplificationspeedTypes)
{
   constexpr speed<meters, seconds> s(1);
   constexpr timespan<minutes> t(1);

   constexpr length l = s * t;

   ASSERT_NEAR(get_length_as<meters>(l), 60.0, TOLERANCE);
}


// TODO: Validate simplification resulting in unitless
// TODO: How to handle unitless?

TEST(Simplification, Validate_constexpr_behavior) {

    using namespace dimension;
    using std::tuple;
    using std::cout;
    using std::endl;

    constexpr base_dimension<unit_exponent<meters>> obj1(10.0);
    constexpr base_dimension<unit_exponent<feet>> obj2(5.0);

    static_assert(get_dimension_as<unit_exponent<meters>>(obj1) == 10.0, "Fail");
    static_assert(get_length_as<meters>(obj1) == 10.0, "Fail");
    static_assert(obj1 > obj2, "Fail");
    static_assert(obj2 < obj1, "Fail");

    constexpr auto obj3 = obj1 + obj2;
    constexpr auto obj4 = obj1 - obj2;
    constexpr auto obj5 = obj1 * obj2;
    constexpr auto obj6 = obj1 / obj2;


    static_assert(obj3 > obj1, "Fail");
    static_assert(obj1 > obj4, "Fail");
    
    static_assert(get_dimension_as<unit_exponent<meters, 2>>(obj5) > 10.0, "Fail");
    static_assert(get_dimension_as<>(obj6) > 0.0, "Fail");
    static_assert(obj6 > 0.0, "Fail");

    constexpr length<meters> obj7(15.0);
    static_assert(obj7 > obj1, "Fail");

    constexpr length obj8 = obj1;
    static_assert(obj8 == obj1, "Fail");

    constexpr speed<meters, seconds> speed1(10.0);
    static_assert(get_dimension_as<unit_exponent<meters>, unit_exponent<seconds, -1>>(speed1) == 10.0, "Fail");

    constexpr energy<joules> energy1(5.0);
    constexpr energy<kilo_grams, meters, seconds> energy2(5.0);

    static_assert(energy1 == energy2, "Fail");
   
}

