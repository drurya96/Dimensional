#include "TestOperators.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"

using namespace Dimension;
using namespace std;

TEST_F(OperatorsTest, Comparisons) {

   Speed mySpeed1(10.0, &LengthUnits::Meters, &TimeUnits::Seconds); // 10.0 m/s == 1968.504 f/m
   Speed mySpeed2(20.0, &LengthUnits::Feet, &TimeUnits::Minutes); // 20.0 f/m == 0.1016 m/s
   Speed mySpeed3(1968.504, &LengthUnits::Feet, &TimeUnits::Minutes);

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

   Length myLength(10.0, &LengthUnits::Meters);
   Time myTime(5.0, &TimeUnits::Seconds);

   // The following operator should fail to compile, not sure how to unit test a compile time failure though
   // Speed mySpeed = myLength * myTime;

   auto myTest = myLength * myTime;

   ASSERT_DOUBLE_EQ(myTest.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters, &TimeUnits::Seconds}, vector<BaseUnit<>*>{}), 50.0);
   ASSERT_DOUBLE_EQ(myTest.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet, &TimeUnits::Minutes}, vector<BaseUnit<>*>{}), 2.73403333333333333);

   ASSERT_TRUE((is_same<decltype(myTest), BaseDimension<LengthUnit<>, TimeUnit<>>>::value));
}

TEST_F(OperatorsTest, DimensionDivision) {

   Length myLength(20.0, &LengthUnits::Meters);
   Time myTime(2.0, &TimeUnits::Seconds);

   // The following operator should fail to compile, not sure how to unit test a compile time failure though
   //BaseDimension<LengthUnit<>, TimeUnit<>> = myLength / myTime;

   auto myTest = myLength / myTime;

   ASSERT_DOUBLE_EQ(myTest.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 10.0);
   ASSERT_DOUBLE_EQ(myTest.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 1968.504);

   ASSERT_TRUE((is_same<decltype(myTest), BaseDimension<LengthUnit<>, TimeUnit<Inverse>>>::value));
   ASSERT_NO_THROW(Speed mySpeed = myTest); // TODO: This is really a test of the cast operator
}

// Test Scalar multiplation
TEST_F(OperatorsTest, ScalarMultiplication)
{
   Speed mySpeed(5.0, &LengthUnits::Meters, &TimeUnits::Seconds);

   mySpeed = mySpeed * 2;

   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 10.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 32.8084);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 600.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 1968.504);

   mySpeed *= 2;

   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 20.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 65.6168);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 1200.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 3937.008);
}

// Test Scalar division
TEST_F(OperatorsTest, ScalarDivision)
{
   Speed mySpeed(20.0, &LengthUnits::Meters, &TimeUnits::Seconds);

   mySpeed = mySpeed / 2;

   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 10.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 32.8084);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 600.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 1968.504);

   mySpeed /= 2;

   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 5.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 16.4042);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 300.0);
   ASSERT_DOUBLE_EQ(mySpeed.GetVal(vector<BaseUnit<>*>{&LengthUnits::Feet}, vector<BaseUnit<>*>{&TimeUnits::Minutes}), 984.252);
}

// Test addition
TEST_F(OperatorsTest, DimensionAddition)
{
   Speed speed1(10.0, &LengthUnits::Meters, &TimeUnits::Seconds);
   Speed speed2(10.0, &LengthUnits::Feet, &TimeUnits::Minutes);

   auto speed3 = speed1 + speed2;
   Speed speed4 = speed1 + speed2;

   ASSERT_TRUE((is_same<decltype(speed3), BaseDimension<LengthUnit<>, TimeUnit<Inverse>>>::value));

   ASSERT_DOUBLE_EQ(speed3.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 10.0507999983744); // TODO: Need to validate precision

   speed3 += speed1;

   ASSERT_DOUBLE_EQ(speed3.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 20.050799998374401); // TODO: Need to validate precision
}

// Test subtraction
TEST_F(OperatorsTest, DimensionSubtraction)
{
   Speed speed1(10.0, &LengthUnits::Meters, &TimeUnits::Seconds);
   Speed speed2(10.0, &LengthUnits::Feet, &TimeUnits::Minutes);

   auto speed3 = speed1 - speed2;
   Speed speed4 = speed1 - speed2;

   ASSERT_TRUE((is_same<decltype(speed3), BaseDimension<LengthUnit<>, TimeUnit<Inverse>>>::value));

   ASSERT_DOUBLE_EQ(speed3.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), 9.9492000016256); // TODO: Need to validate precision

   speed3 -= speed1;

   ASSERT_DOUBLE_EQ(speed3.GetVal(vector<BaseUnit<>*>{&LengthUnits::Meters}, vector<BaseUnit<>*>{&TimeUnits::Seconds}), -0.050799998374399635); // TODO: Need to validate precision
}
