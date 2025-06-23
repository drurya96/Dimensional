#include "DimensionTest.h"

#include "timespan_dimension.h"

//#include "base_dimension.h"
#include "length_dimension.h"

#include "speed_dimension.h"

using namespace dimension;

/*
struct IntUnit
{
   using units = std::tuple<unit_exponent<int>>;
};

struct FloatUnit
{
   using units = std::tuple<unit_exponent<float>>;
};

struct DoubleUnit
{
   using units = std::tuple<unit_exponent<double>>;
};

struct SpecialUnit
{
   using units = std::tuple<unit_exponent<float>, unit_exponent<double, -2>>;
};


TEST(TempTest, SimpleObject) {

   base_dimension<unit_exponent<seconds>> obj{10.0};

}

TEST(TempTest, TestApplyPower) {

   using x = unit_exponent<int, 1, 3>;
   using y = unit_exponent<float, 2>;
   using z = unit_exponent<double>;

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
   using a = std::tuple<unit_exponent<IntUnit>>;

   using res = typename FundamentalUnitExtractor<a>::units;

   using res1 = std::tuple_element_t<0, res>;

   ASSERT_TRUE((std::is_same_v<res1::unit, int>));
}

TEST(TempTest, TestFundamentalExtractor) {

   using a = std::tuple<unit_exponent<IntUnit>, unit_exponent<SpecialUnit>>;

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

   
   using a = std::tuple<unit_exponent<IntUnit>, unit_exponent<SpecialUnit>>;

   using extracted = typename FundamentalUnitExtractor<a>::units;
   ASSERT_TRUE((std::is_same_v<extracted, std::tuple<unit_exponent<int>, unit_exponent<float>, unit_exponent<double, -2>>>));

   using res = AddPower<unit_exponent<double>, extracted>::units;
   
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
   
      using a = std::tuple<unit_exponent<IntUnit>, unit_exponent<SpecialUnit>, unit_exponent<FloatUnit, -1>, unit_exponent<DoubleUnit>>;

      using extracted = typename FundamentalUnitExtractor<a>::units;
      ASSERT_TRUE((std::is_same_v<extracted, std::tuple<unit_exponent<int>, unit_exponent<float>, unit_exponent<double, -2>, unit_exponent<float, -1>, unit_exponent<double>>>));

      using resInt = typename CombinePower<int, extracted>::unit;
      using resFloat = typename CombinePower<float, extracted>::unit;
      using resDouble = typename CombinePower<double, extracted>::unit;
      
      ASSERT_TRUE((std::is_same_v<resInt, unit_exponent<int, 1, 1>>));
      ASSERT_TRUE((std::is_same_v<resFloat, unit_exponent<float, 0, 1>>));
      ASSERT_TRUE((std::is_same_v<resDouble, unit_exponent<double, -1, 1>>));

}

TEST(TempTest, TestUnitPresent) {
   
   using a = std::tuple<unit_exponent<int>, unit_exponent<float, -1>>;

   constexpr bool resInt = UnitPresent<int, a>::value;
   constexpr bool resFloat = UnitPresent<float, a>::value;
   constexpr bool resDouble = UnitPresent<double, a>::value;
   
   ASSERT_TRUE(resInt);
   ASSERT_TRUE(resFloat);
   ASSERT_FALSE(resDouble);

}


TEST(TempTest, TestInitialSimplifier) {

   using a = std::tuple<unit_exponent<IntUnit>, unit_exponent<SpecialUnit>, unit_exponent<FloatUnit, -1>, unit_exponent<DoubleUnit>>;

   using res = typename InitialSimplifier<a>::units;
   
   using res1 = std::tuple_element_t<0, res>;
   using res2 = std::tuple_element_t<1, res>;
   
   ASSERT_TRUE((std::is_same_v<res1, unit_exponent<int, 1, 1>>));
   ASSERT_TRUE((std::is_same_v<res2, unit_exponent<double, -1, 1>>));
   
}

//template<> struct Conversion<minutes, seconds> { static constexpr PrecisionType slope = (60.0); };

TEST(TempTest, Testlength) {

   base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>, unit_exponent<minutes>> obj{10.0};

   auto res = FullSimplify(obj);
   using resTunits = typename decltype(res)::units;

   EXPECT_TRUE((std::is_same_v<resTunits, std::tuple<unit_exponent<meters>>>));
   EXPECT_TRUE((std::is_same_v<decltype(res), base_dimension<unit_exponent<meters>>>));

   EXPECT_NEAR(res.GetRaw(), 600.0, 0.1);

}

TEST(TempTest, ConvertDim_HappyPath)
{
   
    using FromTuple = std::tuple<
        unit_exponent<meters>,
        unit_exponent<seconds, -1>
    >;

    using ToTuple = std::tuple<
        unit_exponent<Kilometers>,
        unit_exponent<minutes, -1>
    >;

   using convert = ConvertDim<FromTuple, ToTuple>;

   EXPECT_TRUE((std::is_same_v<typename convert::FromRemaining, std::tuple<unit_exponent<meters>, unit_exponent<seconds, -1>>>));
   EXPECT_TRUE((std::is_same_v<typename convert::ToRemaining, std::tuple<unit_exponent<Kilometers>, unit_exponent<minutes, -1>>>));

    double input = 1800.0; // Meaning 1800 meters per second
    auto result = convert::Convert(input);

    // Let's work out what should happen manually:
    // 1. meters → Kilometers: divide by 1000
    // 2. seconds → minutes: divide by 60
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
   
   base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>> speed(10.0);

   double extracted = get_speed_as<Miles, Hours>(speed);

   EXPECT_NEAR(extracted, 22.3694, 1e-4);

}

/*
TEST(TempTest, ComparisonDifferentUnits) {
   // Setup two speeds
   base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>> speed_mps(10.0);   // 10 meters/sec
   base_dimension<unit_exponent<Kilometers>, unit_exponent<Hours, -1>> faster_speed(40.0); // 40 km/h

   EXPECT_TRUE(speed_mps < faster_speed);
   EXPECT_TRUE(faster_speed > speed_mps);
   EXPECT_TRUE(speed_mps <= faster_speed);
   EXPECT_TRUE(faster_speed >= speed_mps);
}
*/

TEST(TempTest, SimpleMultiply) {
   base_dimension<unit_exponent<meters>> length(10.0);
   base_dimension<unit_exponent<seconds, -1>> inverseTime(1.0 / 5.0);

   auto result = length * inverseTime;

   EXPECT_NEAR((get_dimension_as<unit_exponent<meters>, unit_exponent<seconds, -1>>(result)), 2.0, 1e-4);
   EXPECT_NEAR((get_dimension_as<unit_exponent<Miles>, unit_exponent<Hours, -1>>(result)), 4.47387, 1e-4);

}

TEST(TempTest, SimpleDivide) {
   base_dimension<unit_exponent<meters>> length(10.0);
   base_dimension<unit_exponent<seconds>> time(5.0);

   auto result = length / time;

   EXPECT_NEAR((get_dimension_as<unit_exponent<meters>, unit_exponent<seconds, -1>>(result)), 2.0, 1e-4);
   EXPECT_NEAR((get_dimension_as<unit_exponent<Miles>, unit_exponent<Hours, -1>>(result)), 4.47387, 1e-4);

}

TEST(TempTest, ScalarMathTests) {
   base_dimension<unit_exponent<meters>> len(10.0);

   auto doubled = len * 2.0;
   auto doubled2 = 2.0 * len;
   auto halved = len / 2.0;
   auto inv = 2.0 / len;   

   EXPECT_NEAR(get_length_as<meters>(doubled), 20.0, 1e-6);
   EXPECT_NEAR(get_length_as<meters>(doubled2), 20.0, 1e-6);
   EXPECT_NEAR(get_length_as<meters>(halved), 5.0, 1e-6);
   EXPECT_NEAR((get_dimension_as<unit_exponent<meters, -1>>(inv)), 0.2, 1e-6);
}



TEST(TempTest, AdditionAndSubtractionDifferentUnits) {
   base_dimension<unit_exponent<meters>> length_m(100.0);         // 100 meters
   base_dimension<unit_exponent<Feet>> length_ft(328.084);        // ~100 meters in feet

   auto added = length_m + length_ft;
   auto subtracted = length_m - length_ft;

   // Expect around 200 meters when added (100 + 100)
   EXPECT_NEAR((get_length_as<meters>(added)), 200.0, 1e-2);

   // Expect around 0 meters when subtracted (100 - 100)
   EXPECT_NEAR((get_length_as<meters>(subtracted)), 0.0, 1e-2);
}

TEST(TempTest, SimpleDivideWithNamedDims) {
   length<meters> length(10.0);
   Time<seconds> time(5.0);

   auto result = length / time;

   EXPECT_NEAR((get_speed_as<meters, seconds>(result)), 2.0, 1e-4);
   EXPECT_NEAR((get_speed_as<Miles, Hours>(result)), 4.47387, 1e-4);

}


TEST(TempTest, Multiplyspeed) {
   speed<meters, seconds> speed(10.0);
   Time<seconds> time(5.0);

   auto result = speed * time;

   base_dimension<unit_exponent<Feet>> other = result;

   EXPECT_NEAR((get_length_as<meters>(result)), 50.0, 1e-4); // TODO: Should not need unit_exponent here, just meters.
   //EXPECT_NEAR((getspeed<unit_exponent<meters>, unit_exponent<seconds, -1>>(speed)), 10.0, 1e-4); // TODO: Should not need unit_exponent here, just meters, seconds.
   EXPECT_NEAR((get_speed_as<meters, seconds>(speed)), 10.0, 1e-4); // TODO: Should not need unit_exponent here, just meters, seconds.
}


TEST(TempTest, MatchingTest){

   using A = base_dimension<unit_exponent<meters, 1, 2>, unit_exponent<seconds, -1>>;
   using B = base_dimension<unit_exponent<Feet, 1, 2>, unit_exponent<seconds, -1>>;
   using C = base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>>;
   
   static_assert(matching_dimensions<A, B>, "A and B should match");
   static_assert(!matching_dimensions<A, C>, "A and C should NOT match");

}
