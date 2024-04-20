#include "TestFundamentalConversions.h"

#include "TimeDimension.h"
#include "LengthDimension.h"

TEST_F(FundamentalConversions, TestTime) {
   Dimension::Time myTime = Dimension::Time(30.0, &Dimension::TimeUnits::Seconds);
   
   ASSERT_DOUBLE_EQ(myTime.GetVal(std::vector<Dimension::BaseUnit<>*>{&Dimension::TimeUnits::Seconds}, std::vector<Dimension::BaseUnit<>*>{}), 30.0);
   ASSERT_DOUBLE_EQ(myTime.GetVal(std::vector<Dimension::BaseUnit<>*>{&Dimension::TimeUnits::Minutes}, std::vector<Dimension::BaseUnit<>*>{}), 0.5);
}


TEST_F(FundamentalConversions, TestLength) {
   Dimension::Length myLength = Dimension::Length(10.0, &Dimension::LengthUnits::Meters);

   ASSERT_DOUBLE_EQ(myLength.GetVal(std::vector<Dimension::BaseUnit<>*>{&Dimension::LengthUnits::Meters}, std::vector<Dimension::BaseUnit<>*>{}), 10.0);
   ASSERT_DOUBLE_EQ(myLength.GetVal(std::vector<Dimension::BaseUnit<>*>{&Dimension::LengthUnits::Feet}, std::vector<Dimension::BaseUnit<>*>{}), 32.8084);


   using NumTuple = std::tuple<int, double, float, char, double>;
   using DenTuple = std::tuple<double, char, int>;

   using ExpectedRes = std::tuple<float, double>;

   using test = Dimension::RemoveOneInstance<double, NumTuple>::type;
   using test2 = Dimension::RemoveOneInstance<int, std::tuple<>>::type;
   static_assert(std::is_same_v<test2, std::tuple<>>);

   static_assert(std::is_same_v<test, std::tuple<int, float, char, double>>);
   static_assert(Dimension::has_type<double, NumTuple>());

   //constexpr bool b = Dimension::has_type<int, NumTuple>();
   //std::cout << !b << std::endl;

   //using Result = Dimension::tuple_diff<NumTuple, DenTuple>::type;

   using namespace std;

   using t5 = tuple<bool, bool>;
   using t6 = tuple<bool>;
   using t7 = tuple<>;

   using t = Dimension::tuple_diff<t5, t7>::type;

   static_assert(is_same_v<t, t5>, "");

   using v = Dimension::tuple_diff<tuple<>, tuple<>>::type;

   static_assert(is_same_v<Dimension::RemoveOneInstance<bool, t6>::type, tuple<>>, "");
   using res = Dimension::tuple_diff<t5, t6>::type;
   static_assert(is_same_v<res, tuple<bool>>, "");


   using n = Dimension::tuple_diff<NumTuple, DenTuple>::type;

   static_assert(is_same_v<n, ExpectedRes>, "");


   auto CompDim = Dimension::BaseDimension<std::tuple<Dimension::LengthUnit<>, Dimension::TimeUnit<>>, std::tuple<Dimension::LengthUnit<>>>();

   auto SimpDim = Dimension::SimplifyBaseDimension(CompDim);


}

// Add more test cases as needed
