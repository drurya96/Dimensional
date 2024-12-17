#include "DimensionTest.h"

#include "BaseDimension.h"
#include "LengthDimension.h"

#include "NewLengthUnits.h"

using namespace Dimension;
using namespace std;

TEST_F(DimensionTest, NewDimension) {
   // TODO: Write tests for creating new dimensions from a user's perspective

   Length<Foo> length1{ 100.0 };

   ASSERT_NEAR(getLength<Feet>(length1), 104.485, TOLERANCE);
   ASSERT_NEAR(getLength<Meters>(length1), 31.847133, TOLERANCE);

   Length<Meters> length2{ 20.0 };

   //ASSERT_NEAR(getLength<Baz>(length2), 68.0, TOLERANCE);
   ASSERT_NEAR(getLength<Baz>(length2), 36.0, TOLERANCE);

   Length<BarSub<0>> length3{ 10.0 };
   Length<BarSub<1>> length4{ 20.0 };

   auto result = length3 / length4;
   ASSERT_NEAR((result.GetVal<tuple<BarSub<0>>, tuple<BarSub<1>>>()), 0.5, TOLERANCE);

   auto result2 = length1 / length2;
   ASSERT_NEAR(static_cast<double>(result2), 1.592356, TOLERANCE);

   Length testL = length4;
   auto result3 = length3 / testL;
   ASSERT_NEAR((result3.GetVal<tuple<BarSub<0>>, tuple<BarSub<1>>>()), 0.5, TOLERANCE);

   /// @todo This should move to its own test
   BaseDimension<std::tuple<grault>, std::tuple<>> OtherDimension{ 10.0 };

   auto divideGrault = OtherDimension / length1;

   ASSERT_TRUE((std::is_same_v<decltype(divideGrault), BaseDimension<std::tuple<Bar>, std::tuple<Baz>>>));




   //Length<Fail> failVar{25.0};

   //ASSERT_NEAR(failVar.GetLength<Meters>(), 25.0, TOLERANCE);

}