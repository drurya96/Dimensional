#include "TestNewDimension.h"

#include "StaticBaseDimension.h"
#include "StaticLength.h"

#include "NewLengthUnits.h"

using namespace StaticDimension;
using namespace std;

TEST_F(NewDimensionTest, NewDimension) {
   // TODO: Write tests for creating new dimensions from a user's perspective

   Length<Foo> length1{ 100.0 };

   ASSERT_NEAR(length1.GetLength<Feet>(), 104.485, TOLERANCE);
   ASSERT_NEAR(length1.GetLength<Meters>(), 31.847133, TOLERANCE);

   Length<Meters> length2{ 20.0 };

   ASSERT_NEAR(length2.GetLength<Baz>(), 68.0, TOLERANCE);

   Length<BarSub<0>> length3{ 10.0 };
   Length<BarSub<1>> length4{ 20.0 };

   auto result = length3 / length4;

   auto result2 = length1 / length2;

   Length testL = length4;

   auto result3 = length3 / testL;

   /// @todo This should move to its own test
   BaseDimension<std::tuple<grault>, std::tuple<>> OtherDimension{ 10.0 };

   auto divideGrault = OtherDimension / length1;

   ASSERT_TRUE((std::is_same_v<decltype(divideGrault), BaseDimension<std::tuple<Bar>, std::tuple<Baz>>>));

}