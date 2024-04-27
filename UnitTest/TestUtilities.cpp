#include "TestUtilities.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;
using namespace std;

// This whole file needs to be updated to a real test suite of the utilities
// In its current state, its more of a testing ground for new utilities

TEST_F(UtilitiesTest, Test_tuple_diff) {
   
   using t1 = tuple<int*, float*>;

   using t2 = tuple<LengthUnit*, TimeUnit*>;

   using T1 = tuple_diff<tuple<int*, float*>, tuple<>>::type;

   ASSERT_TRUE((is_same_v<T1, tuple<int*, float*>>));

   using T2 = tuple_diff<tuple<>, tuple<int*, float*>>::type;

   ASSERT_TRUE((is_same_v<T2, tuple<>>));

   using T3 = tuple_diff<tuple<int*, float*>, tuple<int*>>::type;

   ASSERT_TRUE((is_same_v<T3, tuple<float*>>));
 

   using T4 = tuple_diff<tuple<float*, int*, float*>, tuple<int*, float*>>::type;

   ASSERT_TRUE((is_same_v<T4, tuple<float*>>));


   using N = LengthUnit*;

   auto tup = tuple<LengthUnit*>{};

   using A = remove_reference_t<decltype(get<0>(tup))>;


   ASSERT_TRUE((has_type<LengthUnit*, t2>::value));
}
