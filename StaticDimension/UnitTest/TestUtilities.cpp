#include "TestUtilities.h"

#include "StaticTime.h"
#include "StaticLength.h"
//#include "SpeedDimension.h"

using namespace StaticDimension;
using namespace std;

// This whole file needs to be updated to a real test suite of the utilities
// In its current state, its more of a testing ground for new utilities

TEST_F(UtilitiesTest, Test_tuple_diff) {

   Meters m = Meters(25.0);

   ASSERT_NEAR(m.GetValue(), 25.0, TOLERANCE);

   Inches i1 = ConvertValue<Meters, Inches>(m);

   Feet f = ConvertValue<Meters, Feet>(m);

   auto a = f.GetPrimary();

   Inches i2 = ConvertValue<Feet, Inches>(f);

   ASSERT_NEAR(f.GetValue(), 82.021, TOLERANCE);


   Seconds s = Seconds(120.0);

   ASSERT_NEAR(s.GetValue(), 120.0, TOLERANCE);

   Minutes min = ConvertValue<Seconds, Minutes>(s);

   ASSERT_NEAR(min.GetValue(), 2.0, TOLERANCE);


   BaseDimension<std::tuple<Seconds, Minutes>, std::tuple<Feet, Meters>> d =
      BaseDimension<std::tuple<Seconds, Minutes>, std::tuple<Feet, Meters>>(0.0, std::tuple(s, min), std::tuple(f, m));

   ASSERT_NEAR(std::get<1>(d.numList).GetValue(), 2.0, TOLERANCE);
   ASSERT_NEAR(std::get<0>(d.denList).GetValue(), 82.021, TOLERANCE);

   //ASSERT_TRUE(convertible<Meters, Feet>::value);

   //std::cout << convertible<Feet, Meters>::value << std::endl;
   std::cout << std::is_same_v<Meters::Dim, Feet::Dim> << std::endl;
   std::cout << std::is_same_v<decltype(m)::Dim, decltype(f)::Dim> << std::endl;

   std::cout << has_match<Meters, std::tuple<Feet>>::value << std::endl;


   using test1 = static_tuple_diff<std::tuple<Feet>, std::tuple<Minutes>>;
   auto test2 = test1::type{ 10.0 };


   using testType = StaticUnitSimplifier<std::tuple<Feet>, std::tuple<Meters>, std::tuple<Seconds>, std::tuple<Minutes>>;
   auto test5 = testType::newNum{};

   std::tuple<Meters, Feet> t50{10, 20};
   std::tuple<Feet, Meters> t51{30, 40};
   std::tuple<Meters, Feet> t52{};

   AddUnitTuples(t50, t51, t52);


   //using test3 = std::tuple<Feet, Minutes>;

   //auto test4 = test3{};


   //static_assert(std::is_same_v<testType::a, std::tuple<Feet>>);

   //auto test = testType::newNum{ 0.0, 0.0 };

   auto test = BaseDimension<std::tuple<Feet, Meters>, std::tuple<Seconds, Minutes>>(1.0, std::tuple<Feet>{10.0}, std::tuple<Meters>{10.0}, std::tuple<Seconds>{10.0}, std::tuple<Minutes>{10.0});


   auto t1 = BaseDimension<std::tuple<Feet, Meters>, std::tuple<>>(1.0, std::tuple<Feet, Meters>{}, std::tuple<>{});
   auto t2 = BaseDimension<std::tuple<>, std::tuple<Minutes, Seconds>>(1.0, std::tuple<>{}, std::tuple<Minutes, Seconds>{});

   auto t3 = t1 / t2;

   auto t4 = std::tuple<Feet, Minutes>{std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};

   auto t5 = std::tuple<Feet, Seconds, Minutes>{1.0, 5.0, 3.0};
   auto t6 = std::tuple<Minutes>{1.0};

   double val = 1.0;


   

   StaticCancelUnits(t5, t4, val);




   BaseDimension<std::tuple<Meters>, std::tuple<>> t10(1.0, std::tuple<Meters>{10.0}, std::tuple<>{});
   BaseDimension<std::tuple<Meters>, std::tuple<>> t30(1.0, std::tuple<Meters>{5.0}, std::tuple<>{});
   BaseDimension<std::tuple<Seconds>, std::tuple<>> t11(1.0, std::tuple<Seconds>{2.0}, std::tuple<>{});


   using t20 = StaticUnitSimplifier<std::tuple<Meters>, std::tuple<>, std::tuple<Seconds>, std::tuple<Meters>>;

   t20::dimType t21{};

   auto t12 = t10 / t11;

   auto t13 = t12 / t30;
   

   using dType = static_tuple_diff<std::tuple<Seconds, Feet, Minutes>, std::tuple<Meters, Minutes>>;

   dType::type testingSimple{};


   using aType = static_tuple_diff<std::tuple<Seconds, Minutes>, std::tuple<Minutes>>;

   aType::type testingSimple2{};

   using bType = static_tuple_diff<std::tuple<Seconds, Minutes>, std::tuple<Seconds>>;

   bType::type testingSimple3{};

   using cType = static_tuple_diff<std::tuple<Minutes, Seconds>, std::tuple<Seconds>>;

   cType::type testingSimple4{};

   static_assert(has_match<Seconds, std::tuple<Seconds, Minutes>>::value);

   /*
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
   */
}

