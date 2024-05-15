#include "TestOperators.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;
using namespace std;


TEST_F(OperatorsTest, Comparisons) {

   Speed<Meters, Seconds> mySpeed1(10.0); // 10.0 m/s == 1968.504 f/m
   Speed<Feet, Minutes> mySpeed2(20.0); // 20.0 f/m == 0.1016 m/s
   Speed<Feet, Minutes> mySpeed3(1968.504);

   // The ASSERT_GE and ASSERT_LE syntax would work here
   //   but since this is a test of the operators, they are written explicitly
   
   ASSERT_TRUE(mySpeed1 > mySpeed2);
   ASSERT_TRUE(mySpeed2 < mySpeed1);
   ASSERT_TRUE(mySpeed1 == mySpeed3);
   ASSERT_FALSE(mySpeed1 != mySpeed3);
   ASSERT_TRUE(mySpeed1 != mySpeed2);
   ASSERT_FALSE(mySpeed1 == mySpeed2);
   ASSERT_TRUE(mySpeed1 >= mySpeed2 && mySpeed1 >= mySpeed3);
   ASSERT_TRUE(mySpeed2 <= mySpeed1 && mySpeed3 <= mySpeed1);
}


TEST_F(OperatorsTest, DimensionMultiplication) {

   Length<Meters> myLength(10.0);
   Time<Seconds> myTime(5.0);

   // The following operator should fail to compile, not sure how to unit test a compile time failure though
   // Speed mySpeed = myLength * myTime;
   
   auto myTest = myLength * myTime;

   ASSERT_DOUBLE_EQ((myTest.GetVal<std::tuple<Meters, Seconds>, std::tuple<>>()), 50.0);

   ASSERT_DOUBLE_EQ((myTest.GetVal<std::tuple<Feet, Minutes>, std::tuple<>>()), 2.73403333333333333);

   ASSERT_TRUE((is_same_v<decltype(myTest), BaseDimension<tuple<Meters, Seconds>, tuple<>>>));
}


TEST_F(OperatorsTest, DimensionDivision) {

   Length<Meters> myLength(20.0);
   Time<Seconds> myTime(2.0);

   // The following operator should fail to compile, not sure how to unit test a compile time failure though
   //BaseDimension<LengthUnit, TimeUnit> = myLength / myTime;

   auto myTest = myLength / myTime;

   ASSERT_DOUBLE_EQ((myTest.GetVal<std::tuple<Meters>, std::tuple<Seconds>>()), 10.0);
   ASSERT_DOUBLE_EQ((myTest.GetVal<std::tuple<Feet>, std::tuple<Minutes>>()), 1968.504);

   ASSERT_TRUE((is_same_v<decltype(myTest), BaseDimension<tuple<Meters>, tuple<Seconds>>>));
   //ASSERT_NO_THROW(Speed mySpeed = myTest); // TODO: This is really a test of the cast operator
}


// Test Scalar multiplation
TEST_F(OperatorsTest, ScalarMultiplication)
{
   Speed<Meters, Seconds> mySpeed(5.0);

   auto t = mySpeed * 2;

   mySpeed = mySpeed * 2;
   
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Meters, Seconds>()), 10.0);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Seconds>()), 32.8084);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Meters, Minutes>()), 600.0);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Minutes>()), 1968.504);
   
   mySpeed *= 2;
   
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Meters, Seconds>()), 20.0);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Seconds>()), 65.6168);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Meters, Minutes>()), 1200.0);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Minutes>()), 3937.008);
   
}

// Test Scalar division
TEST_F(OperatorsTest, ScalarDivision)
{
   Speed<Meters, Seconds> mySpeed(20.0);

   mySpeed = mySpeed / 2;

   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Meters, Seconds>()), 10.0);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Seconds>()), 32.8084);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Meters, Minutes>()), 600.0);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Minutes>()), 1968.504);

   mySpeed /= 2;

   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Meters, Seconds>()), 5.0);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Seconds>()), 16.4042);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Meters, Minutes>()), 300.0);
   ASSERT_DOUBLE_EQ((mySpeed.GetSpeed<Feet, Minutes>()), 984.252);
}
/*
// Test addition
TEST_F(OperatorsTest, DimensionAddition)
{
   Speed speed1(10.0, &LengthUnits::Meters(), &TimeUnits::Seconds());
   Speed speed2(10.0, &LengthUnits::Feet(), &TimeUnits::Minutes());

   auto speed3 = speed1 + speed2;
   Speed speed4 = speed1 + speed2;

   ASSERT_TRUE((is_same<decltype(speed3), BaseDimension<tuple<LengthUnit*>, tuple<TimeUnit*>>>::value));

   ASSERT_DOUBLE_EQ(speed3.GetVal(UnitList(LengthUnits::Meters()), UnitList(TimeUnits::Seconds())), 10.0507999983744); // TODO: Need to validate precision

   speed3 += speed1;

   ASSERT_DOUBLE_EQ(speed3.GetVal(UnitList(LengthUnits::Meters()), UnitList(TimeUnits::Seconds())), 20.050799998374401); // TODO: Need to validate precision
}

// Test subtraction
TEST_F(OperatorsTest, DimensionSubtraction)
{
   Speed speed1(10.0, &LengthUnits::Meters(), &TimeUnits::Seconds());
   Speed speed2(10.0, &LengthUnits::Feet(), &TimeUnits::Minutes());

   auto speed3 = speed1 - speed2;
   Speed speed4 = speed1 - speed2;

   ASSERT_TRUE((is_same<decltype(speed3), BaseDimension<tuple<LengthUnit*>, tuple<TimeUnit*>>>::value));

   ASSERT_DOUBLE_EQ(speed3.GetVal(UnitList(LengthUnits::Meters()), UnitList(TimeUnits::Seconds())), 9.9492000016256); // TODO: Need to validate precision

   speed3 -= speed1;

   ASSERT_DOUBLE_EQ(speed3.GetVal(UnitList(LengthUnits::Meters()), UnitList(TimeUnits::Seconds())), -0.050799998374399635); // TODO: Need to validate precision
}
*/