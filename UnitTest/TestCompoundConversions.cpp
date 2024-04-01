#include "TestCompoundConversions.h"


#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

TEST_F(CompoundConversions, TestTime) {
   Dimension::Speed mySpeed = Dimension::Speed(30.0, Dimension::SpeedUnits::MetersPerSecond);
   
   //ASSERT_DOUBLE_EQ(myTime.GetVal(&Dimension::TimeUnits::Seconds), 30.0);
   //ASSERT_DOUBLE_EQ(myTime.GetVal(&Dimension::TimeUnits::Minutes), 0.5);

   auto const mySpeed2 = Dimension::Speed(30.0, Dimension::SpeedUnits::MetersPerSecond);

   auto const length = Dimension::Length(20.0, &Dimension::LengthUnits::Feet);
   auto const time = Dimension::Time(10.0, &Dimension::TimeUnits::Seconds);

   //auto const i_time = Dimension::Time(0.5, &Dimension::TimeUnit);

   auto const lengthAsBase = Dimension::BaseDimension<Dimension::LengthUnit<>>(length);
   auto const timeAsBase = Dimension::BaseDimension<Dimension::TimeUnit<>>(time);


   auto speed1 = lengthAsBase / timeAsBase;
   auto speed2 = length / time;

   auto lengthTime = length * time;

   auto speedTime = time * speed2;


   //auto inverseLength = Dimension::Length(15.0, &Dimension::LengthUnits::Inverse_Meters);

   //auto test = Dimension::Simplify(speedTime);

   auto speedTimeTime = speedTime * time;

   auto speedTimeTimePerTime = speedTimeTime / time;

   auto speedTimeTimePerTimeLength = speedTimeTimePerTime * length;

   auto speedTimeLengthTimeTime = speed2 * time * length * time * time;

   auto test = Dimension::SimplifyBaseDimension(speedTimeLengthTimeTime);
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
