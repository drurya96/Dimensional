#include "DimensionTest.h"

#include "TimeDimension.h"

//#include "BaseDimension.h"
#include "LengthDimension.h"

#include "SpeedDimension.h"

using namespace Dimension;

/*
struct IntUnit
{
   using units = std::tuple<UnitExponent<int>>;
};

struct FloatUnit
{
   using units = std::tuple<UnitExponent<float>>;
};

struct DoubleUnit
{
   using units = std::tuple<UnitExponent<double>>;
};

struct SpecialUnit
{
   using units = std::tuple<UnitExponent<float>, UnitExponent<double, -2>>;
};


TEST(TempTest, SimpleObject) {

   BaseDimension<UnitExponent<Seconds>> obj{10.0};

}

TEST(TempTest, TestApplyPower) {

   using x = UnitExponent<int, 1, 3>;
   using y = UnitExponent<float, 2>;
   using z = UnitExponent<double>;

   using r = std::ratio<2>;

   using b = typename ApplyPower<r, std::tuple<x, y, z>>::units;

   using res_exp_x = std::tuple_element_t<0, b>;
   using res_exp_y = std::tuple_element_t<1, b>;
   using res_exp_z = std::tuple_element_t<2, b>;

   ASSERT_TRUE((std::is_same_v<res_exp_x::unit, int>));
   ASSERT_TRUE(res_exp_x::exponent::num == 2);
   ASSERT_TRUE(res_exp_x::exponent::den == 3);

   ASSERT_TRUE((std::is_same_v<res_exp_y::unit, float>));
   ASSERT_TRUE(res_exp_y::exponent::num == 4);
   ASSERT_TRUE(res_exp_y::exponent::den == 1);

   ASSERT_TRUE((std::is_same_v<res_exp_z::unit, double>));
   ASSERT_TRUE(res_exp_z::exponent::num == 2);
   ASSERT_TRUE(res_exp_z::exponent::den == 1);

}

TEST(TempTest, TestFundamentalExtractorSimple) {
   using a = std::tuple<UnitExponent<IntUnit>>;

   using res = typename FundamentalUnitExtractor<a>::units;

   using res1 = std::tuple_element_t<0, res>;

   ASSERT_TRUE((std::is_same_v<res1::unit, int>));
}

TEST(TempTest, TestFundamentalExtractor) {

   using a = std::tuple<UnitExponent<IntUnit>, UnitExponent<SpecialUnit>>;

   using res = typename FundamentalUnitExtractor<a>::units;

   using res1 = std::tuple_element_t<0, res>;
   using res2 = std::tuple_element_t<1, res>;
   using res3 = std::tuple_element_t<2, res>;

   ASSERT_TRUE((std::is_same_v<res1::unit, int>));
   ASSERT_TRUE((std::is_same_v<res2::unit, float>));
   ASSERT_TRUE((std::is_same_v<res3::unit, double>));

   ASSERT_TRUE((std::is_same_v<res1::exponent, std::ratio<1,1>>));
   ASSERT_TRUE((std::is_same_v<res2::exponent, std::ratio<1,1>>));
   ASSERT_TRUE((std::is_same_v<res3::exponent, std::ratio<-2,1>>));

}

TEST(TempTest, TestAddingPower) {

   
   using a = std::tuple<UnitExponent<IntUnit>, UnitExponent<SpecialUnit>>;

   using extracted = typename FundamentalUnitExtractor<a>::units;
   ASSERT_TRUE((std::is_same_v<extracted, std::tuple<UnitExponent<int>, UnitExponent<float>, UnitExponent<double, -2>>>));

   using res = AddPower<UnitExponent<double>, extracted>::units;
   
   using res1 = std::tuple_element_t<0, res>;
   using res2 = std::tuple_element_t<1, res>;
   using res3 = std::tuple_element_t<2, res>;

   ASSERT_TRUE((std::is_same_v<res1::unit, int>));
   ASSERT_TRUE((std::is_same_v<res2::unit, float>));
   ASSERT_TRUE((std::is_same_v<res3::unit, double>));

   ASSERT_TRUE((std::is_same_v<res1::exponent, std::ratio<1,1>>));
   ASSERT_TRUE((std::is_same_v<res2::exponent, std::ratio<1,1>>));
   ASSERT_TRUE((std::is_same_v<res3::exponent, std::ratio<-1,1>>));

}

TEST(TempTest, TestCombinePower) {
   
      using a = std::tuple<UnitExponent<IntUnit>, UnitExponent<SpecialUnit>, UnitExponent<FloatUnit, -1>, UnitExponent<DoubleUnit>>;

      using extracted = typename FundamentalUnitExtractor<a>::units;
      ASSERT_TRUE((std::is_same_v<extracted, std::tuple<UnitExponent<int>, UnitExponent<float>, UnitExponent<double, -2>, UnitExponent<float, -1>, UnitExponent<double>>>));

      using resInt = typename CombinePower<int, extracted>::unit;
      using resFloat = typename CombinePower<float, extracted>::unit;
      using resDouble = typename CombinePower<double, extracted>::unit;
      
      ASSERT_TRUE((std::is_same_v<resInt, UnitExponent<int, 1, 1>>));
      ASSERT_TRUE((std::is_same_v<resFloat, UnitExponent<float, 0, 1>>));
      ASSERT_TRUE((std::is_same_v<resDouble, UnitExponent<double, -1, 1>>));

}

TEST(TempTest, TestUnitPresent) {
   
   using a = std::tuple<UnitExponent<int>, UnitExponent<float, -1>>;

   constexpr bool resInt = UnitPresent<int, a>::value;
   constexpr bool resFloat = UnitPresent<float, a>::value;
   constexpr bool resDouble = UnitPresent<double, a>::value;
   
   ASSERT_TRUE(resInt);
   ASSERT_TRUE(resFloat);
   ASSERT_FALSE(resDouble);

}


TEST(TempTest, TestInitialSimplifier) {

   using a = std::tuple<UnitExponent<IntUnit>, UnitExponent<SpecialUnit>, UnitExponent<FloatUnit, -1>, UnitExponent<DoubleUnit>>;

   using res = typename InitialSimplifier<a>::units;
   
   using res1 = std::tuple_element_t<0, res>;
   using res2 = std::tuple_element_t<1, res>;
   
   ASSERT_TRUE((std::is_same_v<res1, UnitExponent<int, 1, 1>>));
   ASSERT_TRUE((std::is_same_v<res2, UnitExponent<double, -1, 1>>));
   
}

//template<> struct Conversion<Minutes, Seconds> { static constexpr PrecisionType slope = (60.0); };

TEST(TempTest, TestLength) {

   BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds, -1>, UnitExponent<Minutes>> obj{10.0};

   auto res = FullSimplify(obj);
   using resTunits = typename decltype(res)::units;

   EXPECT_TRUE((std::is_same_v<resTunits, std::tuple<UnitExponent<Meters>>>));
   EXPECT_TRUE((std::is_same_v<decltype(res), BaseDimension<UnitExponent<Meters>>>));

   EXPECT_NEAR(res.GetRaw(), 600.0, 0.1);

}

TEST(TempTest, ConvertDim_HappyPath)
{
   
    using FromTuple = std::tuple<
        UnitExponent<Meters>,
        UnitExponent<Seconds, -1>
    >;

    using ToTuple = std::tuple<
        UnitExponent<KiloMeters>,
        UnitExponent<Minutes, -1>
    >;

   using convert = ConvertDim<FromTuple, ToTuple>;

   EXPECT_TRUE((std::is_same_v<typename convert::FromRemaining, std::tuple<UnitExponent<Meters>, UnitExponent<Seconds, -1>>>));
   EXPECT_TRUE((std::is_same_v<typename convert::ToRemaining, std::tuple<UnitExponent<KiloMeters>, UnitExponent<Minutes, -1>>>));

    double input = 1800.0; // Meaning 1800 meters per second
    auto result = convert::Convert(input);

    // Let's work out what should happen manually:
    // 1. Meters → Kilometers: divide by 1000
    // 2. Seconds → Minutes: divide by 60
    //
    // Overall scaling:
    //  (input / 1000) / (1/60) = (input / 1000) * 60
    //                         = input * (60 / 1000)
    //                         = input * 0.06
    //
    // Thus:
    // expected = 1800 * 0.06 = 108.0

    EXPECT_NEAR(result, 108.0, 1e-6);

}
*/


TEST(TempTest, TestRealDim)
{
   
   BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds, -1>> speed(10.0);

   double extracted = get_speed_as<Miles, Hours>(speed);

   EXPECT_NEAR(extracted, 22.3694, 1e-4);

}

/*
TEST(TempTest, ComparisonDifferentUnits) {
   // Setup two speeds
   BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds, -1>> speed_mps(10.0);   // 10 meters/sec
   BaseDimension<UnitExponent<KiloMeters>, UnitExponent<Hours, -1>> faster_speed(40.0); // 40 km/h

   EXPECT_TRUE(speed_mps < faster_speed);
   EXPECT_TRUE(faster_speed > speed_mps);
   EXPECT_TRUE(speed_mps <= faster_speed);
   EXPECT_TRUE(faster_speed >= speed_mps);
}
*/

TEST(TempTest, SimpleMultiply) {
   BaseDimension<UnitExponent<Meters>> length(10.0);
   BaseDimension<UnitExponent<Seconds, -1>> inverseTime(1.0 / 5.0);

   auto result = length * inverseTime;

   EXPECT_NEAR((get_dimension_as<UnitExponent<Meters>, UnitExponent<Seconds, -1>>(result)), 2.0, 1e-4);
   EXPECT_NEAR((get_dimension_as<UnitExponent<Miles>, UnitExponent<Hours, -1>>(result)), 4.47387, 1e-4);

}

TEST(TempTest, SimpleDivide) {
   BaseDimension<UnitExponent<Meters>> length(10.0);
   BaseDimension<UnitExponent<Seconds>> time(5.0);

   auto result = length / time;

   EXPECT_NEAR((get_dimension_as<UnitExponent<Meters>, UnitExponent<Seconds, -1>>(result)), 2.0, 1e-4);
   EXPECT_NEAR((get_dimension_as<UnitExponent<Miles>, UnitExponent<Hours, -1>>(result)), 4.47387, 1e-4);

}

TEST(TempTest, ScalarMathTests) {
   BaseDimension<UnitExponent<Meters>> len(10.0);

   auto doubled = len * 2.0;
   auto doubled2 = 2.0 * len;
   auto halved = len / 2.0;
   auto inv = 2.0 / len;   

   EXPECT_NEAR(get_length_as<Meters>(doubled), 20.0, 1e-6);
   EXPECT_NEAR(get_length_as<Meters>(doubled2), 20.0, 1e-6);
   EXPECT_NEAR(get_length_as<Meters>(halved), 5.0, 1e-6);
   EXPECT_NEAR((get_dimension_as<UnitExponent<Meters, -1>>(inv)), 0.2, 1e-6);
}



TEST(TempTest, AdditionAndSubtractionDifferentUnits) {
   BaseDimension<UnitExponent<Meters>> length_m(100.0);         // 100 meters
   BaseDimension<UnitExponent<Feet>> length_ft(328.084);        // ~100 meters in feet

   auto added = length_m + length_ft;
   auto subtracted = length_m - length_ft;

   // Expect around 200 meters when added (100 + 100)
   EXPECT_NEAR((get_length_as<Meters>(added)), 200.0, 1e-2);

   // Expect around 0 meters when subtracted (100 - 100)
   EXPECT_NEAR((get_length_as<Meters>(subtracted)), 0.0, 1e-2);
}

TEST(TempTest, SimpleDivideWithNamedDims) {
   Length<Meters> length(10.0);
   Time<Seconds> time(5.0);

   auto result = length / time;

   EXPECT_NEAR((get_speed_as<Meters, Seconds>(result)), 2.0, 1e-4);
   EXPECT_NEAR((get_speed_as<Miles, Hours>(result)), 4.47387, 1e-4);

}


TEST(TempTest, MultiplySpeed) {
   Speed<Meters, Seconds> speed(10.0);
   Time<Seconds> time(5.0);

   auto result = speed * time;

   BaseDimension<UnitExponent<Feet>> other = result;

   EXPECT_NEAR((get_length_as<Meters>(result)), 50.0, 1e-4); // TODO: Should not need UnitExponent here, just Meters.
   //EXPECT_NEAR((getSpeed<UnitExponent<Meters>, UnitExponent<Seconds, -1>>(speed)), 10.0, 1e-4); // TODO: Should not need UnitExponent here, just Meters, Seconds.
   EXPECT_NEAR((get_speed_as<Meters, Seconds>(speed)), 10.0, 1e-4); // TODO: Should not need UnitExponent here, just Meters, Seconds.
}


TEST(TempTest, MatchingTest){

   using A = BaseDimension<UnitExponent<Meters, 1, 2>, UnitExponent<Seconds, -1>>;
   using B = BaseDimension<UnitExponent<Feet, 1, 2>, UnitExponent<Seconds, -1>>;
   using C = BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds, -1>>;
   
   static_assert(matching_dimensions<A, B>, "A and B should match");
   static_assert(!matching_dimensions<A, C>, "A and C should NOT match");

}
