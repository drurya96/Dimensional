#include "TestCompoundConversions.h"


#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

TEST_F(CompoundConversions, TestTime) {

   //using namespace Dimension;

   //using input = std::tuple<int, double, int, char, double>;

   //using expected = std::tuple<std::tuple<int, int>, std::tuple<double, double>, std::tuple<char>>;

   //using result = Squish<input>::type;

   //static_assert(std::is_same<result, expected>::value, "Wrong");

   //auto test = BreakTypes<TimeUnit<>, TimeUnit<>, TimeUnit<Inverse>, LengthUnit<>>{};

   //using breakType = BreakTypes_t<TimeUnit<>, TimeUnit<>, TimeUnit<Inverse>, LengthUnit<>>;

   //auto result = split_types<int, double, int, char, double>();

   //auto breakType = split_types<TimeUnit<>, TimeUnit<>, TimeUnit<Inverse>, LengthUnit<>>();
   //using expected = std::tuple<
   //                     std::tuple<TimeUnit<>, TimeUnit<>, TimeUnit<Inverse>>,
   //                     std::tuple<LengthUnit<>>
   //                 >;

   //static_assert(std::is_same<breakType, expected>::value, "Wrong");

   //std::cout << result << std::endl;

   //result a{};

   //ASSERT_EQ(0, 0);

   
   Dimension::Speed mySpeed = Dimension::Speed(30.0, &Dimension::LengthUnits::Meters, &Dimension::TimeUnits::Seconds);
    
   Dimension::Time myTime = Dimension::Time(2, &Dimension::TimeUnits::Minutes);

   Dimension::Length myLength = mySpeed * myTime;

   Dimension::Length myNewLength1 = 10.0 * myLength;
   Dimension::Length myNewLength2 = myLength * 10.0;

   auto secondPerMeter = myTime / myLength;

   auto secondPerMeter2 = secondPerMeter / 0.5;
   Dimension::Speed myNewSpeed = 5 / secondPerMeter;
   

   //ASSERT_DOUBLE_EQ(myTime.GetVal(&Dimension::TimeUnits::Seconds), 30.0);
   //ASSERT_DOUBLE_EQ(myTime.GetVal(&Dimension::TimeUnits::Minutes), 0.5);

   //auto const mySpeed2 = Dimension::Speed(30.0, Dimension::SpeedUnits::MetersPerSecond);

   auto length = Dimension::Length(20.0, &Dimension::LengthUnits::Feet);
   auto length2 = Dimension::Length(20.0, &Dimension::LengthUnits::Meters);
   
   auto const time = Dimension::Time(10.0, &Dimension::TimeUnits::Seconds);

   //auto const i_time = Dimension::Time(0.5, &Dimension::TimeUnit);

   auto const lengthAsBase = Dimension::BaseDimension<Dimension::LengthUnit<>>(length);
   auto const timeAsBase = Dimension::BaseDimension<Dimension::TimeUnit<>>(time);


   //auto speed1 = lengthAsBase / timeAsBase;
   auto test = length / time * time;

   Dimension::Length<> newTest = Dimension::Length<>(test);
   Dimension::Length newTest1 = Dimension::Length(test);

   Dimension::Length test2 = time / time * length * length / length;

   Dimension::Length test3 = length + length2;
   Dimension::Length test4 = length2 + length;

   length += length2;

   std::cout << "Numerator unit: " << newTest1.numList[0]->unitName << std::endl;

   //auto test = Dimension::SimplifyBaseDimension(speed2);

   //auto lengthTime = length * time;

   //auto speedTime = time * speed2;


   //auto inverseLength = Dimension::Length(15.0, &Dimension::LengthUnits::Inverse_Meters);

   //auto test = Dimension::Simplify(speedTime);

   //auto speedTimeTime = speedTime * time;

   //auto speedTimeTimePerTime = speedTimeTime / time;

   //auto speedTimeTimePerTimeLength = speedTimeTimePerTime * length;

   //auto speedTimeLengthTimeTime = speed2 * time * length * time * time;

   //auto test = Dimension::SimplifyBaseDimension(speedTimeLengthTimeTime);


   //auto time2 = Dimension::Time(2.0, &Dimension::TimeUnits::Minutes);

   //auto newTest = speed2 * time2;

   //auto newTest2 = Dimension::SimplifyBaseDimension(newTest);
   

   //auto unwrapped = Dimension::UnwrappedTuple<decltype(test)>{};

   //Dimension::BaseDimension<Dimension::LengthUnit<>> test = Dimension::BaseDimension<>::BubbleSort(Dimension::BaseDimension<Dimension::LengthUnit<>>());
   //auto test = Dimension::BaseDimension<>::Sort(Dimension::BaseDimension<Dimension::LengthUnit<>, Dimension::TimeUnit<>>());
   //auto test = Dimension::UpdateOrder(Dimension::BaseDimension<Dimension::LengthUnit<>>());

   //auto test = Dimension::BaseDimension<>::BubbleSort(speedTime);
   //auto test = Dimension::UpdateOrder(speedTime);
   //auto test = Dimension::UpdateOrder(length);

   //auto newLength = Dimension::SimplifyDimension(speedTime);

   //Dimension::BaseDimension<Dimension::LengthUnit<>, Dimension::TimeUnit<Dimension::Inverse>> speed = Dimension::myMethod(length, time);

   //auto test = Dimension::myMethod(length, Dimension::BaseDimension<Dimension::TimeUnit<Dimension::Inverse>>());

   //auto test = Dimension::invertDimension(Dimension::TimeUnits::Seconds);

   //auto test = Dimension::myMethod(time, mySpeed2);
   //auto test = Dimension::myMethod(lengthAsBase, timeAsBase);

   
   //Dimension::BaseDimension<Dimension::LengthUnit<>> length2 = Dimension::BaseDimension<Dimension::LengthUnit<>>();
   //Dimension::BaseDimension<Dimension::TimeUnit<>> time2 = Dimension::BaseDimension<Dimension::TimeUnit<>>();
   
   //auto speed2 = Dimension::myMethod(length2, time2);

   //Dimension::Speed speed2 = length / time;

   //ASSERT_DOUBLE_EQ(speed2.value, 2.0);


   //using namespace Dimension;

   //BaseDimension<LengthUnit<>, TimeUnit<>, TimeUnit<Inverse>, LengthUnit<>> myNewDimObject;


}

// Add more test cases as needed
