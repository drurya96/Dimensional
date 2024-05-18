#include "TestNewDimension.h"

#include "StaticBaseDimension.h"
#include "StaticLength.h"

#include "NewLengthUnits.h"

using namespace StaticDimension;
using namespace std;

TEST_F(NewDimensionTest, NewDimension) {
   // TODO: Write tests for creating new dimensions from a user's perspective

   Length<Yards> length1{ 100.0 };

   ASSERT_NEAR(length1.GetLength<Feet>(), 300.0, TOLERANCE);
   ASSERT_NEAR(length1.GetLength<Meters>(), 91.44, TOLERANCE);

}