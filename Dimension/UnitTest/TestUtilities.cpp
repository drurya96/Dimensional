#include "TestUtilities.h"

#include "BaseDimension.h"
#include "DimensionUtilities.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"

using namespace Dimension;
using namespace std;

TEST_F(UtilitiesTest, Test_tuple_cat_t)
{
   ASSERT_TRUE((is_same_v<tuple_cat_t<tuple<int>, tuple<double>>, tuple<int, double>>));
   ASSERT_TRUE((is_same_v<tuple_cat_t<tuple<int, double>, tuple<double, int>>, tuple<int, double, double, int>>));
   ASSERT_TRUE((is_same_v<tuple_cat_t<tuple<int, double>, tuple<>>, tuple<int, double>>));
}


TEST_F(UtilitiesTest, Test_same_dim)
{
   ASSERT_TRUE((is_same_dim<Meters, Meters>::value));
   ASSERT_TRUE((is_same_dim<Meters, Feet>::value));
   ASSERT_FALSE((is_same_dim<Meters, Seconds>::value));

   ASSERT_TRUE((has_same_dim<Meters, tuple<Seconds, Meters>>::value));
   ASSERT_TRUE((has_same_dim<Meters, tuple<Seconds, Feet>>::value));
   ASSERT_FALSE((has_same_dim<Meters, tuple<Seconds, Minutes>>::value));
}

TEST_F(UtilitiesTest, Test_remove_instance)
{
   
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, Meters, tuple<Meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, Meters, tuple<Feet>>::type, tuple<>>));
   // Attempting to remove Meters removes the first Length type, which is Feet in this case
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, Meters, tuple<Feet, Seconds, Meters>>::type, tuple<Seconds, Meters>>));

   // Nothing to remove...
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, Meters, tuple<Seconds>>::type, tuple<Seconds>>));

   // Remove from empty tuple... no-op
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, Meters, tuple<>>::type, tuple<>>));
   
}

TEST_F(UtilitiesTest, Test_GetConvertedValue)
{
   using fromNum = tuple<Meters, Seconds>;
   using fromDen = tuple<Feet, Grams>;

   using toNum = tuple<Feet, Minutes>;
   using toDen = tuple<Inches, Ounces>;

   PrecisionType value = 10.0;
   
   // Convert only the numerator, to the same type. This is a no-op
   ConvertDimension<0, false, fromNum, fromNum, true>(value);
   ASSERT_DOUBLE_EQ(value, 10.0);

   // Convert only the denominator, to the same type. This is a no-op
   ConvertDimension<0, true, fromDen, fromDen, true>(value);
   ASSERT_DOUBLE_EQ(value, 10.0);
   
   // Convert only the numerator
   ConvertDimension<0, false, toNum, fromNum, true>(value);
   ASSERT_NEAR(value, 0.5468, TOLERANCE);

   value = 10.0; // reset value

   // Convert only the denominator
   ConvertDimension<0, true, toDen, fromDen, true>(value);
   ASSERT_NEAR(value, 23.6246, TOLERANCE);
   
}

TEST_F(UtilitiesTest, Test_tuple_diff_dim)
{
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<Meters>, tuple<Meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<Meters>, tuple<Meters, Seconds>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<Meters, Seconds>, tuple<Seconds, Meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<Meters, Seconds>, tuple<Meters>>::type, tuple<Seconds>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<Meters, Seconds, Meters>, tuple<Seconds>>::type, tuple<Meters, Meters>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<>, tuple<Seconds>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<>, tuple<>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<Feet>, tuple<Meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<Feet, Meters>, tuple<Meters>>::type, tuple<Meters>>));
}

TEST_F(UtilitiesTest, Test_UnitSimplifier)
{
   // IMPORTANT NOTE
   // At least for now, UnitSimplifier *assumes* NumTuple1 - DenTuple1 and NumTuple2 - DenTuple2
   //    are already simplified relative to each other.
   //    This means the struct may not behave as expected if NumTuple1 contains a matching type
   //    with DenTuple1, and the same logic applies for NumTuple2 and DenTuple2
   //
   // Undecided whether this behavior with be changed. This is a user-facing behavior so user-feedback
   //    may dictate how this situation is handled.

   using simplified1 = UnitSimplifier<tuple<Meters>, tuple<Seconds>, tuple<Minutes>, tuple<Grams>>;
   ASSERT_TRUE((is_same_v<simplified1::newNum, tuple<Meters>>));
   ASSERT_TRUE((is_same_v<simplified1::newDen, tuple<Grams>>));
   ASSERT_TRUE((is_same_v<simplified1::dimType, BaseDimension<tuple<Meters>, tuple<Grams>>>));

   using simplified2 = UnitSimplifier<tuple<Meters>, tuple<Seconds>, tuple<Seconds>, tuple<Meters>>;
   ASSERT_TRUE((is_same_v<simplified2::newNum, tuple<>>));
   ASSERT_TRUE((is_same_v<simplified2::newDen, tuple<>>));

   using simplified3 = UnitSimplifier<tuple<Meters>, tuple<Feet>, tuple<>, tuple<Meters>>;
   ASSERT_TRUE((is_same_v<simplified3::newNum, tuple<Feet>>));
   ASSERT_TRUE((is_same_v<simplified3::newDen, tuple<>>));

   using simplified4 = UnitSimplifier<tuple<Meters, Grams, Meters, Feet>, tuple<Ounces, Seconds>, tuple<Hours>, tuple<Inches, Seconds, Seconds>>;
   ASSERT_TRUE((is_same_v<simplified4::newNum, tuple<Grams, Meters, Feet, Ounces>>));
   ASSERT_TRUE((is_same_v<simplified4::newDen, tuple<Seconds, Seconds>>));
}

TEST_F(UtilitiesTest, Test_CancelUnits)
{
   using NumTup1 = tuple<Meters, Seconds, Grams>;
   using DenTup1 = tuple<Feet, Ounces>;

   using simplified1 = UnitSimplifier<NumTup1, tuple<>, tuple<>, DenTup1>;

   PrecisionType value = 1.0; // This is used to track the value. Consider adding another utility for this

   CancelUnits<NumTup1, DenTup1, simplified1::newNum, simplified1::newDen, true>(value);

   //ASSERT_NEAR(value, 0.11572835, TOLERANCE);
}
