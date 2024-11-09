#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"
#include "AngleDimension.h"

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


TEST_F(DimensionTest, TestFunctionParameters) {

   Length<Meters> ret1 = TestFunction1(Time<Seconds>(5.0));
   ASSERT_NEAR((getLength<Meters>(ret1)), 5.0, TOLERANCE);

   Length<Meters> ret2 = TestFunction1(Time<Minutes>(1.0));
   ASSERT_NEAR((getLength<Meters>(ret2)), 60.0, TOLERANCE);
   
   Length<KiloMeters> ret3 = TestFunction1(Time<Seconds>(5.0));
   ASSERT_NEAR((getLength<Meters>(ret3)), 5.0, TOLERANCE);

   Time<Minutes> test(1.0);

   double ret4 = TestFunction2(test);
   ASSERT_NEAR(ret4, 60.0, TOLERANCE);

}
