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

   using test = Dimension::RemoveOneInstance<double, NumTuple>::type;

   static_assert(std::is_same_v<test, std::tuple<int, float, char, double>>);
   static_assert(Dimension::has_type<double, NumTuple>());

   //constexpr bool b = Dimension::has_type<int, NumTuple>();
   //std::cout << !b << std::endl;

   //using Result = Dimension::tuple_diff<NumTuple, DenTuple>::type;

   using namespace std;

   using t5 = tuple<bool, bool>;
   using t6 = tuple<bool>;
   //using res = Dimension::tuple_diff<t5, t6>::type;
   //static_assert(is_same_v<Dimension::tuple_diff<t5, t6>::type, tuple<bool>>, "");

}

// Add more test cases as needed
