#include "DimensionTest.h"

using namespace dimension;
using namespace std;

TEST(NewDimension, NewDimension) {
   // TODO: Write tests for creating new dimensions from a user's perspective

   length<Foo> length1{ 100.0 };

   ASSERT_NEAR(getlength<Feet>(length1), 104.485, TOLERANCE);
   ASSERT_NEAR(getlength<meters>(length1), 31.847133, TOLERANCE);

   length<meters> length2{ 20.0 };

   //ASSERT_NEAR(getlength<Baz>(length2), 68.0, TOLERANCE);
   ASSERT_NEAR(getlength<Baz>(length2), 36.0, TOLERANCE);

   length<BarSub<0>> length3{ 10.0 };
   length<BarSub<1>> length4{ 20.0 };

   auto result = length3 / length4;
   ASSERT_NEAR((result.GetVal<tuple<BarSub<0>>, tuple<BarSub<1>>>()), 0.5, TOLERANCE);

   auto result2 = length1 / length2;
   ASSERT_NEAR(static_cast<double>(result2), 1.592356, TOLERANCE);

   length testL = length4;
   auto result3 = length3 / testL;
   ASSERT_NEAR((result3.GetVal<tuple<BarSub<0>>, tuple<BarSub<1>>>()), 0.5, TOLERANCE);

   /// @todo This should move to its own test
   base_dimension<std::tuple<grault>, std::tuple<>> OtherDimension{ 10.0 };

   auto divideGrault = OtherDimension / length1;

   ASSERT_TRUE((std::is_same_v<decltype(divideGrault), base_dimension<std::tuple<Bar>, std::tuple<Baz>>>));




   //length<Fail> failVar{25.0};

   //ASSERT_NEAR(failVar.Getlength<meters>(), 25.0, TOLERANCE);

}