#include "DimensionTest.h"

#include <iostream>

using namespace dimension;

length<meters> TestFunction1(timespan<seconds> time)
{
   return length<meters>(get_timespan_as<seconds>(time));
}

length<meters> TestFunction1(length<meters> time)
{
   return length<meters>(get_length_as<meters>(time));
}


template<typename T>
double TestFunction2(timespan<T> time)
{
   return get_timespan_as<seconds>(time);
}

template<typename T>
double TestFunction2(length<T> time)
{
   return get_length_as<meters>(time);
}

double TestFunctionGenericlength(dimension::force<> force)
{
   return get_force_as<newtons>(force);
}

namespace dimension
{
   struct NonBaseUnit : FundamentalUnitTag
   {
      using Dim = lengthType;
      using Primary = meters;

      using units = std::tuple<unit_exponent<NonBaseUnit>>;

      // cppcheck-suppress unusedStructMember
      constexpr static int ID = 0;
   };

   template<> struct Conversion<NonBaseUnit,  meters> { static constexpr PrecisionType slope = 3.14; };
}

TEST(Functions, TestFunctionParameters) {

   length<meters> ret1 = TestFunction1(timespan<seconds>(5.0));
   ASSERT_NEAR((get_length_as<meters>(ret1)), 5.0, TOLERANCE);

   length<meters> ret2 = TestFunction1(timespan<minutes>(1.0));
   ASSERT_NEAR((get_length_as<meters>(ret2)), 60.0, TOLERANCE);
  
   length<kilo_meters> ret3 = TestFunction1(timespan<seconds>(5.0));
   ASSERT_NEAR((get_length_as<meters>(ret3)), 5.0, TOLERANCE);

   length<NonBaseUnit> X(1.0);
   ASSERT_NEAR((get_length_as<meters>(X)), 3.14, TOLERANCE);

   timespan<minutes> test(1.0);

   double ret4 = TestFunction2(test);
   ASSERT_NEAR(ret4, 60.0, TOLERANCE);

   force<grams, miles, hours> f1(10000000.0);
   ASSERT_NEAR(TestFunctionGenericlength(f1), 1.24178, TOLERANCE);

}

//struct celsiusT : public reference_frame<kelvin>{ static constexpr double offset = 273.15; };

TEST(Functions, PlaceholderTest1) {


   point<celsius, temperatureType> a{10.0};

   double val = get_point_as<celsius>(a);

   ASSERT_NEAR(val, 10.0, 0.0000001);

   val = get_point_as<kelvin>(a);

   ASSERT_NEAR(val, 283.15, 0.0000001);

   val = get_point_as<fahrenheit>(a);

   ASSERT_NEAR(val, 50.0, 0.0000001);

   point<kelvin, temperatureType> b{283.15};

   val = get_point_as<celsius>(b);

   ASSERT_NEAR(val, 10.0, 0.0000001);

   val = get_point_as<kelvin>(b);

   ASSERT_NEAR(val, 283.15, 0.0000001);

}

TEST(Functions, PlaceholderTest2) {

   //point<celsius, temperatureType> t1{25.0};
   point<kelvin, temperatureType> t1{298.15};
   point<celsius, temperatureType> t2{10.0};
   temperature<kelvin> dT{10.0};

   

   point<celsius, temperatureType> res1 = t1 + dT;
   EXPECT_NEAR(get_point_as<celsius>(res1), 35.0, 0.0000001);

   point<celsius, temperatureType> res2 = dT + t1;
   EXPECT_NEAR(get_point_as<celsius>(res2), 35.0, 0.0000001);

   point<celsius, temperatureType> res3 = t1 - dT;
   EXPECT_NEAR(get_point_as<celsius>(res3), 15.0, 0.0000001);

   temperature<kelvin> res4 = t1 - t2;
   EXPECT_NEAR(get_temperature_as<kelvin>(res4), 15.0, 0.0000001);


   auto a = timespan<seconds>(2.0) / dT;
   EXPECT_NEAR((get_dimension_as<unit_exponent<seconds>, unit_exponent<kelvin, -1>>(a)), 0.2, 0.0000001);

   auto a1 = t1 * a;
   EXPECT_NEAR(get_timespan_as<seconds>(a1), 59.63, 0.0000001);

   auto a2 = a * t1;
   EXPECT_NEAR(get_timespan_as<seconds>(a2), 59.63, 0.0000001);

   
   auto b = timespan<seconds>(2.0) * dT;
   EXPECT_NEAR((get_dimension_as<unit_exponent<seconds>, unit_exponent<kelvin>>(b)), 20.0, 0.0000001);

   auto b1 = b / t1;
   EXPECT_NEAR(get_timespan_as<seconds>(b1), 0.06708032869361059869193359047459, 0.0000001);


   auto c = dT / timespan<seconds>(2.0);
   EXPECT_NEAR((get_dimension_as<unit_exponent<kelvin>, unit_exponent<seconds, -1>>(c)), 5.0, 0.0000001);

   auto c1 = t1 / c;
   EXPECT_NEAR(get_timespan_as<seconds>(c1), 59.63, 0.0000001);

}