#include "DimensionTest.h"

using namespace dimension;
using namespace std;

TEST(Utilities, Test_tuple_cat_t)
{
   ASSERT_TRUE((is_same_v<tuple_cat_t<tuple<int>, tuple<double>>, tuple<int, double>>));
   ASSERT_TRUE((is_same_v<tuple_cat_t<tuple<int, double>, tuple<double, int>>, tuple<int, double, double, int>>));
   ASSERT_TRUE((is_same_v<tuple_cat_t<tuple<int, double>, tuple<>>, tuple<int, double>>));
}


TEST(Utilities, Test_same_dim)
{
   ASSERT_TRUE((is_same_dim<meters, meters>::value));
   ASSERT_TRUE((is_same_dim<meters, Feet>::value));
   ASSERT_FALSE((is_same_dim<meters, seconds>::value));

   ASSERT_TRUE((has_same_dim<meters, tuple<seconds, meters>>::value));
   ASSERT_TRUE((has_same_dim<meters, tuple<seconds, Feet>>::value));
   ASSERT_FALSE((has_same_dim<meters, tuple<seconds, minutes>>::value));
}

TEST(Utilities, Test_remove_instance)
{
   
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, meters, tuple<meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, meters, tuple<Feet>>::type, tuple<>>));
   // Attempting to remove meters removes the first length type, which is Feet in this case
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, meters, tuple<Feet, seconds, meters>>::type, tuple<seconds, meters>>));

   // Nothing to remove...
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, meters, tuple<seconds>>::type, tuple<seconds>>));

   // Remove from empty tuple... no-op
   ASSERT_TRUE((is_same_v<RemoveOneInstance<is_same_dim, meters, tuple<>>::type, tuple<>>));
   
}

TEST(Utilities, Test_GetConvertedValue)
{
   using fromNum = tuple<meters, seconds>;
   using fromDen = tuple<Feet, Grams>;

   using toNum = tuple<Feet, minutes>;
   using toDen = tuple<Inches, Ounces>;

   constexpr PrecisionType value = 10.0;
   
   // Convert only the numerator, to the same type. This is a no-op
   ASSERT_DOUBLE_EQ((ConvertDimension<0, false, fromNum, fromNum, true>(value)), 10.0);

   // Convert only the denominator, to the same type. This is a no-op
   ASSERT_DOUBLE_EQ((ConvertDimension<0, true, fromDen, fromDen, true>(value)), 10.0);
   
   // Convert only the numerator
   ASSERT_NEAR((ConvertDimension<0, false, toNum, fromNum, true>(value)), 0.5468, TOLERANCE);

   // Convert only the denominator
   ASSERT_NEAR((ConvertDimension<0, true, toDen, fromDen, true>(value)), 23.6246, TOLERANCE);
   
}

TEST(Utilities, Test_tuple_diff_dim)
{
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<meters>, tuple<meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<meters>, tuple<meters, seconds>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<meters, seconds>, tuple<seconds, meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<meters, seconds>, tuple<meters>>::type, tuple<seconds>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<meters, seconds, meters>, tuple<seconds>>::type, tuple<meters, meters>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<>, tuple<seconds>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<>, tuple<>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<Feet>, tuple<meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<has_same_dim, tuple<Feet, meters>, tuple<meters>>::type, tuple<meters>>));
}

TEST(Utilities, Test_UnitSimplifier)
{
   // IMPORTANT NOTE
   // At least for now, UnitSimplifier *assumes* NumTuple1 - DenTuple1 and NumTuple2 - DenTuple2
   //    are already simplified relative to each other.
   //    This means the struct may not behave as expected if NumTuple1 contains a matching type
   //    with DenTuple1, and the same logic applies for NumTuple2 and DenTuple2
   //
   // Undecided whether this behavior with be changed. This is a user-facing behavior so user-feedback
   //    may dictate how this situation is handled.

   using simplified1 = UnitSimplifier<tuple<meters>, tuple<seconds>, tuple<minutes>, tuple<Grams>>;
   ASSERT_TRUE((is_same_v<simplified1::newNum, tuple<meters>>));
   ASSERT_TRUE((is_same_v<simplified1::newDen, tuple<Grams>>));
   ASSERT_TRUE((is_same_v<simplified1::dimType, base_dimension<tuple<meters>, tuple<Grams>>>));

   using simplified2 = UnitSimplifier<tuple<meters>, tuple<seconds>, tuple<seconds>, tuple<meters>>;
   ASSERT_TRUE((is_same_v<simplified2::newNum, tuple<>>));
   ASSERT_TRUE((is_same_v<simplified2::newDen, tuple<>>));

   using simplified3 = UnitSimplifier<tuple<meters>, tuple<Feet>, tuple<>, tuple<meters>>;
   ASSERT_TRUE((is_same_v<simplified3::newNum, tuple<Feet>>));
   ASSERT_TRUE((is_same_v<simplified3::newDen, tuple<>>));

   using simplified4 = UnitSimplifier<tuple<meters, Grams, meters, Feet>, tuple<Ounces, seconds>, tuple<Hours>, tuple<Inches, seconds, seconds>>;
   ASSERT_TRUE((is_same_v<simplified4::newNum, tuple<Grams, meters, Feet, Ounces>>));
   ASSERT_TRUE((is_same_v<simplified4::newDen, tuple<seconds, seconds>>));
}

TEST(Utilities, Test_CancelUnits)
{
   using NumTup1 = tuple<meters, seconds, Grams>;
   using DenTup1 = tuple<Feet, Ounces>;

   using simplified1 = UnitSimplifier<NumTup1, tuple<>, tuple<>, DenTup1>;

   PrecisionType value = 1.0; // This is used to track the value. Consider adding another utility for this

   CancelUnits<NumTup1, DenTup1, simplified1::newNum, simplified1::newDen, true>(value);

   //ASSERT_NEAR(value, 0.11572835, TOLERANCE);
}
