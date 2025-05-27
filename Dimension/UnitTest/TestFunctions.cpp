#include "DimensionTest.h"

#include "BaseDimension.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"
#include "AngleDimension.h"
#include "ForceDimension.h"

#include <iostream>

#include "TemperatureDimension.h"

using namespace Dimension;

Length<Meters> TestFunction1(Time<Seconds> time)
{
   return Length<Meters>(get_time_as<Seconds>(time));
}

Length<Meters> TestFunction1(Length<Meters> time)
{
   return Length<Meters>(get_length_as<Meters>(time));
}


template<typename T>
double TestFunction2(Time<T> time)
{
   return get_time_as<Seconds>(time);
}

template<typename T>
double TestFunction2(Length<T> time)
{
   return get_length_as<Meters>(time);
}

double TestFunctionGenericLength(Dimension::Force<> force)
{
   return get_force_as<Newtons>(force);
}

namespace Dimension
{
   struct NonBaseUnit : FundamentalUnitTag
   {
      using Dim = LengthType;
      using Primary = Meters;

      using units = std::tuple<UnitExponent<NonBaseUnit>>;

      // cppcheck-suppress unusedStructMember
      constexpr static int ID = 0;
   };

   template<> struct Conversion<NonBaseUnit,  Meters> { static constexpr PrecisionType slope = 3.14; };
}

TEST(Functions, TestFunctionParameters) {

   Length<Meters> ret1 = TestFunction1(Time<Seconds>(5.0));
   ASSERT_NEAR((get_length_as<Meters>(ret1)), 5.0, TOLERANCE);

   Length<Meters> ret2 = TestFunction1(Time<Minutes>(1.0));
   ASSERT_NEAR((get_length_as<Meters>(ret2)), 60.0, TOLERANCE);
  
   Length<KiloMeters> ret3 = TestFunction1(Time<Seconds>(5.0));
   ASSERT_NEAR((get_length_as<Meters>(ret3)), 5.0, TOLERANCE);

   Length<NonBaseUnit> X(1.0);
   ASSERT_NEAR((get_length_as<Meters>(X)), 3.14, TOLERANCE);

   Time<Minutes> test(1.0);

   double ret4 = TestFunction2(test);
   ASSERT_NEAR(ret4, 60.0, TOLERANCE);

   Force<Grams, Miles, Hours> f1(10000000.0);
   ASSERT_NEAR(TestFunctionGenericLength(f1), 1.24178, TOLERANCE);

}

//struct CelsiusT : public reference_frame<Kelvin>{ static constexpr double offset = 273.15; };

TEST(Functions, PlaceholderTest1) {


   point<Celsius, TemperatureType> a{10.0};

   double val = get_point_as<Celsius>(a);

   ASSERT_NEAR(val, 10.0, 0.0000001);

   val = get_point_as<Kelvin>(a);

   ASSERT_NEAR(val, 283.15, 0.0000001);

   val = get_point_as<Fahrenheit>(a);

   ASSERT_NEAR(val, 50.0, 0.0000001);

   point<Kelvin, TemperatureType> b{283.15};

   val = get_point_as<Celsius>(b);

   ASSERT_NEAR(val, 10.0, 0.0000001);

   val = get_point_as<Kelvin>(b);

   ASSERT_NEAR(val, 283.15, 0.0000001);

}

TEST(Functions, PlaceholderTest2) {

   //point<Celsius, TemperatureType> t1{25.0};
   point<Kelvin, TemperatureType> t1{298.15};
   point<Celsius, TemperatureType> t2{10.0};
   Temperature<Kelvin> dT{10.0};

   

   point<Celsius, TemperatureType> res1 = t1 + dT;
   EXPECT_NEAR(get_point_as<Celsius>(res1), 35.0, 0.0000001);

   point<Celsius, TemperatureType> res2 = dT + t1;
   EXPECT_NEAR(get_point_as<Celsius>(res2), 35.0, 0.0000001);

   point<Celsius, TemperatureType> res3 = t1 - dT;
   EXPECT_NEAR(get_point_as<Celsius>(res3), 15.0, 0.0000001);

   Temperature<Kelvin> res4 = t1 - t2;
   EXPECT_NEAR(get_temperature_as<Kelvin>(res4), 15.0, 0.0000001);


   auto a = Time<Seconds>(2.0) / dT;
   EXPECT_NEAR((get_dimension_as<UnitExponent<Seconds>, UnitExponent<Kelvin, -1>>(a)), 0.2, 0.0000001);

   auto a1 = t1 * a;
   EXPECT_NEAR(get_time_as<Seconds>(a1), 59.63, 0.0000001);

   auto a2 = a * t1;
   EXPECT_NEAR(get_time_as<Seconds>(a2), 59.63, 0.0000001);

   
   auto b = Time<Seconds>(2.0) * dT;
   EXPECT_NEAR((get_dimension_as<UnitExponent<Seconds>, UnitExponent<Kelvin>>(b)), 20.0, 0.0000001);

   auto b1 = b / t1;
   EXPECT_NEAR(get_time_as<Seconds>(b1), 0.06708032869361059869193359047459, 0.0000001);


   auto c = dT / Time<Seconds>(2.0);
   EXPECT_NEAR((get_dimension_as<UnitExponent<Kelvin>, UnitExponent<Seconds, -1>>(c)), 5.0, 0.0000001);

   auto c1 = t1 / c;
   EXPECT_NEAR(get_time_as<Seconds>(c1), 59.63, 0.0000001);

}