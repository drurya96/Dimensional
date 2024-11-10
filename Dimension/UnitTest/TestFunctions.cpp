#include "DimensionTest.h"

#include "BaseDimension.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"
#include "AngleDimension.h"

#include <iostream>

using namespace Dimension;

Length<Meters> TestFunction1(Time<Seconds> time)
{
   return Length<Meters>(getTime<Seconds>(time));
}

Length<Meters> TestFunction1(Length<Meters> time)
{
   return Length<Meters>(getLength<Meters>(time));
}


template<typename T>
double TestFunction2(Time<T> time)
{
   return getTime<Seconds>(time);
}

template<typename T>
double TestFunction2(Length<T> time)
{
   return getLength<Meters>(time);
}


template<typename T>
void PrintUnit()
{
   std::cout << T::name << "; " << T::abbr << std::endl;
}

namespace Dimension
{
   struct NonBaseUnit
   {
      using Dim = LengthType;
      using Primary = Meters;

      using NumTuple = std::tuple<NonBaseUnit>;
      using DenTuple = std::tuple<>;

      constexpr static int ID = 0;
   };

   template<> struct Conversion<NonBaseUnit,  Meters> { static constexpr PrecisionType slope = 3.14; };
}

TEST_F(DimensionTest, TestFunctionParameters) {

   Length<Meters> ret1 = TestFunction1(Time<Seconds>(5.0));
   ASSERT_NEAR((getLength<Meters>(ret1)), 5.0, TOLERANCE);

   Length<Meters> ret2 = TestFunction1(Time<Minutes>(1.0));
   ASSERT_NEAR((getLength<Meters>(ret2)), 60.0, TOLERANCE);
  
   Length<KiloMeters> ret3 = TestFunction1(Time<Seconds>(5.0));
   ASSERT_NEAR((getLength<Meters>(ret3)), 5.0, TOLERANCE);

   Length<NonBaseUnit> X(1.0);
   ASSERT_NEAR((getLength<Meters>(X)), 3.14, TOLERANCE);

   Time<Minutes> test(1.0);

   double ret4 = TestFunction2(test);
   ASSERT_NEAR(ret4, 60.0, TOLERANCE);

   PrintUnit<Seconds>();
   PrintUnit<MilliSeconds>();
   PrintUnit<MegaGrams>();
   PrintUnit<Radians>();
   PrintUnit<DataMiles>();

}
