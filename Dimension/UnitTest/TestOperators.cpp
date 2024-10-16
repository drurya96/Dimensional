#include "TestOperators.h"

#include "StaticTime.h"
#include "StaticLength.h"
#include "StaticSpeed.h"

using namespace StaticDimension;
using namespace std;


TEST_F(OperatorsTest, Comparisons) {

   Speed<Meters, Seconds> mySpeed1(10.0); // 10.0 m/s == 1968.504 f/m
   Speed<Feet, Minutes> mySpeed2(20.0); // 20.0 f/m == 0.1016 m/s
   Speed<Feet, Minutes> mySpeed3(1968.5);
   Speed<Meters, Seconds> mySpeedZero1(0.0);
   Speed<Feet, Minutes> mySpeedZero2(0.0);

   std::cout << mySpeed1.GetSpeed<Feet, Minutes>() << std::endl;
   std::cout << mySpeed3.GetSpeed<Meters, Seconds>() << std::endl;

   // The ASSERT_GE and ASSERT_LE syntax would work here
   //   but since this is a test of the operators, they are written explicitly
   
   ASSERT_TRUE(mySpeed1 > mySpeed2);
   ASSERT_TRUE(mySpeed2 < mySpeed1);
   ASSERT_TRUE(mySpeed1.NearlyEqual(mySpeed3, 0.001));
   ASSERT_FALSE(mySpeedZero2 != mySpeedZero1);
   ASSERT_TRUE(mySpeedZero1 == mySpeedZero2);
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

   ASSERT_NEAR((myTest.GetVal<std::tuple<Meters, Seconds>, std::tuple<>>()), 50.0, TOLERANCE);

   ASSERT_NEAR((myTest.GetVal<std::tuple<Feet, Minutes>, std::tuple<>>()), 2.73403333333333333, TOLERANCE);

   ASSERT_TRUE((is_same_v<decltype(myTest), BaseDimension<tuple<Meters, Seconds>, tuple<>>>));
}


TEST_F(OperatorsTest, DimensionDivision) {

   Length<Meters> myLength(20.0);
   Time<Seconds> myTime(2.0);

   // The following operator should fail to compile, not sure how to unit test a compile time failure though
   //BaseDimension<LengthUnit, TimeUnit> = myLength / myTime;

   auto myTest = myLength / myTime;

   ASSERT_NEAR((myTest.GetVal<std::tuple<Meters>, std::tuple<Seconds>>()), 10.0, TOLERANCE);
   ASSERT_NEAR((myTest.GetVal<std::tuple<Feet>, std::tuple<Minutes>>()), 1968.504, TOLERANCE);

   ASSERT_TRUE((is_same_v<decltype(myTest), BaseDimension<tuple<Meters>, tuple<Seconds>>>));
   //ASSERT_NO_THROW(Speed mySpeed = myTest); // TODO: This is really a test of the cast operator
}


// Test Scalar multiplation
TEST_F(OperatorsTest, ScalarMultiplication)
{
   Speed<Meters, Seconds> mySpeed(5.0);

   auto t = mySpeed * 2;

   mySpeed = mySpeed * 2;
   
   ASSERT_NEAR((mySpeed.GetSpeed<Meters, Seconds>()), 10.0, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Seconds>()), 32.8084, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Meters, Minutes>()), 600.0, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Minutes>()), 1968.504, TOLERANCE);
   
   mySpeed *= 2;
   
   ASSERT_NEAR((mySpeed.GetSpeed<Meters, Seconds>()), 20.0, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Seconds>()), 65.6168, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Meters, Minutes>()), 1200.0, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Minutes>()), 3937.008, TOLERANCE);
   
}

// Test Scalar division
TEST_F(OperatorsTest, ScalarDivision)
{
   Speed<Meters, Seconds> mySpeed(20.0);

   mySpeed = mySpeed / 2;

   ASSERT_NEAR((mySpeed.GetSpeed<Meters, Seconds>()), 10.0, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Seconds>()), 32.8084, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Meters, Minutes>()), 600.0, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Minutes>()), 1968.504, TOLERANCE);

   mySpeed /= 2;

   ASSERT_NEAR((mySpeed.GetSpeed<Meters, Seconds>()), 5.0, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Seconds>()), 16.4042, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Meters, Minutes>()), 300.0, TOLERANCE);
   ASSERT_NEAR((mySpeed.GetSpeed<Feet, Minutes>()), 984.252, TOLERANCE);
}

// Test addition
TEST_F(OperatorsTest, DimensionAddition)
{
   Speed<Meters, Seconds> speed1(10.0);
   Speed<Feet, Minutes> speed2(10.0);

   auto speed3 = speed1 + speed2;
   Speed speed4 = speed1 + speed2;

   ASSERT_TRUE((is_same<decltype(speed3), BaseDimension<tuple<Meters>, tuple<Seconds>>>::value));

   ASSERT_NEAR((speed4.GetSpeed<Meters, Seconds>()), 10.0507999983744, TOLERANCE); // TODO: Need to validate precision

   speed4 += speed1;

   ASSERT_NEAR((speed4.GetSpeed<Meters, Seconds>()), 20.050799998374401, TOLERANCE); // TODO: Need to validate precision
}

// Test subtraction
TEST_F(OperatorsTest, DimensionSubtraction)
{
   Speed<Meters, Seconds> speed1(10.0);
   Speed<Feet, Minutes> speed2(10.0);

   auto speed3 = speed1 - speed2;
   Speed speed4 = speed1 - speed2;

   ASSERT_TRUE((is_same<decltype(speed3), BaseDimension<tuple<Meters>, tuple<Seconds>>>::value));

   ASSERT_NEAR((speed4.GetSpeed<Meters, Seconds>()), 9.9492000016256, TOLERANCE); // TODO: Need to validate precision

   speed4 -= speed1;

   ASSERT_NEAR((speed4.GetSpeed<Meters, Seconds>()), -0.050799998374399635, TOLERANCE); // TODO: Need to validate precision
}

// Test Exponent
TEST_F(OperatorsTest, DimensionExponent)
{
   Speed<Meters, Seconds> speed(10.0);

   BaseDimension<std::tuple<Meters, Meters, Meters>, std::tuple<Seconds, Seconds, Seconds>> test1 = speed * speed * speed;

   auto test2 = StaticDimension::Pow<3>(speed);

   ASSERT_TRUE(test1 == test2);
}
