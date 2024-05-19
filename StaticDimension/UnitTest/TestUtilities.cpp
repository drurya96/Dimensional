#include "TestUtilities.h"

#include "StaticBaseDimension.h"
#include "StaticDimensionUtilities.h"

#include "StaticTime.h"
#include "StaticLength.h"
#include "StaticMass.h"

using namespace StaticDimension;
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
   ASSERT_TRUE((is_same_v<RemoveOneInstance<Meters, tuple<Meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<RemoveOneInstance<Meters, tuple<Feet>>::type, tuple<>>));
   // Attempting to remove Meters removes the first Length type, which is Feet in this case
   ASSERT_TRUE((is_same_v<RemoveOneInstance<Meters, tuple<Feet, Seconds, Meters>>::type, tuple<Seconds, Meters>>));

   // Nothing to remove...
   ASSERT_TRUE((is_same_v<RemoveOneInstance<Meters, tuple<Seconds>>::type, tuple<Seconds>>));

   // Remove from empty tuple... no-op
   ASSERT_TRUE((is_same_v<RemoveOneInstance<Meters, tuple<>>::type, tuple<>>));
}

TEST_F(UtilitiesTest, Test_GetConvertedValue)
{
   using fromNum = tuple<Meters, Seconds>;
   using fromDen = tuple<Feet, Grams>;

   using toNum = tuple<Feet, Minutes>;
   using toDen = tuple<Inches, Ounces>;

   double value = 10.0;

   // Converting from units of value 1.0 (simple)
   typename fromNum OrigNum{ 1.0, 1.0 };
   typename fromDen OrigDen{ 1.0, 1.0 };

   // Convert only the numerator, to the same type. This is a no-op
   GetConvertedValue<0, false, fromNum>(OrigNum, value);
   ASSERT_DOUBLE_EQ(value, 10.0);

   // Convert only the denominator, to the same type. This is a no-op
   GetConvertedValue<0, true, fromDen>(OrigDen, value);
   ASSERT_DOUBLE_EQ(value, 10.0);

   // Convert only the numerator
   GetConvertedValue<0, false, toNum>(OrigNum, value);
   ASSERT_NEAR(value, 0.5468, TOLERANCE);

   value = 10.0; // reset value

   // Convert only the denominator
   GetConvertedValue<0, true, toDen>(OrigDen, value);
   ASSERT_NEAR(value, 23.6246, TOLERANCE);


   // Converting from units of real values
   typename fromNum OrigNum2{ 2.0, 5.0 };
   typename fromDen OrigDen2{ 3.0, 12.0 };

   // Convert only the numerator
   GetConvertedValue<0, false, toNum>(OrigNum2, value);
   ASSERT_NEAR(value, 12.9181, TOLERANCE);

   value = 10.0; // reset value

   // Convert only the denominator
   GetConvertedValue<0, true, toDen>(OrigDen2, value);
   ASSERT_NEAR(value, 0.65624, TOLERANCE);
}

TEST_F(UtilitiesTest, Test_tuple_diff)
{
   ASSERT_TRUE((is_same_v<tuple_diff<tuple<Meters>, tuple<Meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<tuple<Meters>, tuple<Meters, Seconds>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<tuple<Meters, Seconds>, tuple<Seconds, Meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<tuple<Meters, Seconds>, tuple<Meters>>::type, tuple<Seconds>>));
   ASSERT_TRUE((is_same_v<tuple_diff<tuple<Meters, Seconds, Meters>, tuple<Seconds>>::type, tuple<Meters, Meters>>));
   ASSERT_TRUE((is_same_v<tuple_diff<tuple<>, tuple<Seconds>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<tuple<>, tuple<>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<tuple<Feet>, tuple<Meters>>::type, tuple<>>));
   ASSERT_TRUE((is_same_v<tuple_diff<tuple<Feet, Meters>, tuple<Meters>>::type, tuple<Meters>>));
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

TEST_F(UtilitiesTest, Test_UpdateRealUnits)
{
   using NumTup1 = tuple<Meters, Seconds, Grams>;
   NumTup1 RealNumTup1{};


   UpdateRealUnits(Meters{ 2.0 }, RealNumTup1);
   UpdateRealUnits(Seconds{ 10.0 }, RealNumTup1);
   UpdateRealUnits(Grams{ 5.0 }, RealNumTup1);
   double value = 1.0; // This is used to track the value. Consider adding another utility for this
   GetConvertedValue<0, false, NumTup1>(RealNumTup1, value);

   ASSERT_NEAR(value, 100.0, TOLERANCE);
}

TEST_F(UtilitiesTest, Test_CancelUnits)
{
   using NumTup1 = tuple<Meters, Seconds, Grams>;
   NumTup1 RealNumTup1{};
   NumTup1 IncomingNumTup1{2.0, 10.0, 5.0};

   using DenTup1 = tuple<Feet, Ounces>;
   DenTup1 RealDenTup1{};
   DenTup1 IncomingDenTup1{3.0, 15.0};


   double value = 1.0; // This is used to track the value. Consider adding another utility for this

   CancelUnits(IncomingNumTup1, IncomingDenTup1, RealNumTup1, RealDenTup1, value);

   
   GetConvertedValue<0, false, NumTup1>(RealNumTup1, value);
   GetConvertedValue<0, true, DenTup1>(RealDenTup1, value);

   ASSERT_NEAR(value, 2.22222, TOLERANCE);
}

TEST_F(UtilitiesTest, Test_AddUnits)
{
   using tup1T = tuple<Feet, Minutes>;
   using tup2T = tuple<Meters, Seconds>;
   tup1T tup1{ 10, 5 };
   tup2T tup2{ 2, 60 };

   tup1T outTup1{};
   tup2T outTup2{};

   AddUnitTuples(tup1, tup2, outTup1);
   AddUnitTuples(tup2, tup1, outTup2);

   double value = 1.0; // This is used to track the value. Consider adding another utility for this
   GetConvertedValue<0, false, tup1T>(outTup1, value);
   ASSERT_NEAR(value, 99.3701, TOLERANCE);

   value = 1.0; // This is used to track the value. Consider adding another utility for this
   GetConvertedValue<0, false, tup2T>(outTup2, value);
   ASSERT_NEAR(value, 1817.28, TOLERANCE);
}
