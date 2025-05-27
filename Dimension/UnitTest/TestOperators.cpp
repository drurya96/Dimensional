#include "DimensionTest.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "SpeedDimension.h"
#include "TemperatureDimension.h"

using namespace Dimension;
using namespace std;


TEST(Operators, Comparisons) {

   Speed<Meters, Seconds> mySpeed1(10.0); // 10.0 m/s == 1968.504 f/m
   Speed<Feet, Minutes> mySpeed2(20.0); // 20.0 f/m == 0.1016 m/s
   Speed<Feet, Minutes> mySpeed3(1968.5);
   Speed<Meters, Seconds> mySpeedZero1(0.0);
   Speed<Feet, Minutes> mySpeedZero2(0.0);

   std::cout << get_speed_as<Feet, Minutes>(mySpeed1) << std::endl;
   std::cout << get_speed_as<Meters, Seconds>(mySpeed3) << std::endl;

   // The ASSERT_GE and ASSERT_LE syntax would work here
   //   but since this is a test of the operators, they are written explicitly
   
   ASSERT_TRUE(mySpeed1 > mySpeed2);
   
   ASSERT_TRUE(mySpeed2 < mySpeed1);
   //ASSERT_TRUE(mySpeed1.NearlyEqual(mySpeed3, 0.001)); TODO: Reconsider NearlyEqual
   ASSERT_FALSE(mySpeedZero2 != mySpeedZero1);
   ASSERT_TRUE(mySpeedZero1 == mySpeedZero2);
   ASSERT_TRUE(mySpeed1 != mySpeed2);
   ASSERT_FALSE(mySpeed1 == mySpeed2);
   ASSERT_TRUE(mySpeed1 >= mySpeed2 && mySpeed1 >= mySpeed3);
   ASSERT_TRUE(mySpeed2 <= mySpeed1 && mySpeed3 <= mySpeed1); 
   
}



TEST(Operators, DimensionMultiplication) {

   Length<Meters> myLength(10.0);
   Time<Seconds> myTime(5.0);

   // The following operator should fail to compile, not sure how to unit test a compile time failure though
   // Speed mySpeed = myLength * myTime;
   
   auto myTest = myLength * myTime;

   ASSERT_NEAR((get_dimension_as<UnitExponent<Meters>, UnitExponent<Seconds>>(myTest)), 50.0, TOLERANCE);

   ASSERT_NEAR((get_dimension_as<UnitExponent<Feet>, UnitExponent<Minutes>>(myTest)), 2.73403333333333333, TOLERANCE);

   ASSERT_TRUE((is_same_v<decltype(myTest), BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds>>>));
}


TEST(Operators, DimensionDivision) {

   Length<Meters> myLength(20.0);
   Time<Seconds> myTime(2.0);

   // The following operator should fail to compile, not sure how to unit test a compile time failure though
   //BaseDimension<LengthUnit, TimeUnit> = myLength / myTime;

   auto myTest = myLength / myTime;

   ASSERT_NEAR((get_dimension_as<UnitExponent<Meters>, UnitExponent<Seconds, -1>>(myTest)), 10.0, TOLERANCE);
   ASSERT_NEAR((get_dimension_as<UnitExponent<Feet>, UnitExponent<Minutes, -1>>(myTest)), 1968.504, TOLERANCE);

   ASSERT_TRUE((is_same_v<decltype(myTest), BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds, -1>>>));
   //ASSERT_NO_THROW(Speed mySpeed = myTest); // TODO: This is really a test of the cast operator
}


// Test Scalar multiplation
TEST(Operators, ScalarMultiplication)
{
   Speed<Meters, Seconds> mySpeed(5.0);

   mySpeed = mySpeed * 2;
   
   ASSERT_NEAR((get_speed_as<Meters, Seconds>(mySpeed)), 10.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Feet, Seconds>(mySpeed)), 32.8084, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Meters, Minutes>(mySpeed)), 600.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Feet, Minutes>(mySpeed)), 1968.504, TOLERANCE);
   
   mySpeed *= 2;
   
   ASSERT_NEAR((get_speed_as<Meters, Seconds>(mySpeed)), 20.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Feet, Seconds>(mySpeed)), 65.6168, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Meters, Minutes>(mySpeed)), 1200.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Feet, Minutes>(mySpeed)), 3937.008, TOLERANCE);
   
}

// Test Scalar division
TEST(Operators, ScalarDivision)
{
   Speed<Meters, Seconds> mySpeed(20.0);

   mySpeed = mySpeed / 2;

   ASSERT_NEAR((get_speed_as<Meters, Seconds>(mySpeed)), 10.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Feet, Seconds>(mySpeed)), 32.8084, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Meters, Minutes>(mySpeed)), 600.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Feet, Minutes>(mySpeed)), 1968.504, TOLERANCE);

   mySpeed /= 2;

   ASSERT_NEAR((get_speed_as<Meters, Seconds>(mySpeed)), 5.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Feet, Seconds>(mySpeed)), 16.4042, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Meters, Minutes>(mySpeed)), 300.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<Feet, Minutes>(mySpeed)), 984.252, TOLERANCE);
}

// Test addition
TEST(Operators, DimensionAddition)
{
   Speed<Meters, Seconds> speed1(10.0);
   Speed<Feet, Minutes> speed2(10.0);

   auto speed3 = speed1 + speed2;
   Speed speed4 = speed1 + speed2;

   ASSERT_TRUE((is_same<decltype(speed3), BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds, -1>>>::value));

   ASSERT_NEAR((get_speed_as<Meters, Seconds>(speed4)), 10.0507999983744, TOLERANCE); // TODO: Need to validate precision

   speed4 += speed1;
   ASSERT_NEAR((get_speed_as<Meters, Seconds>(speed4)), 20.050799998374401, TOLERANCE); // TODO: Need to validate precision
}

// Test subtraction
TEST(Operators, DimensionSubtraction)
{
   Speed<Meters, Seconds> speed1(10.0);
   Speed<Feet, Minutes> speed2(10.0);

   auto speed3 = speed1 - speed2;
   Speed speed4 = speed1 - speed2;

   ASSERT_TRUE((is_same<decltype(speed3), BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds, -1>>>::value));

   ASSERT_NEAR((get_speed_as<Meters, Seconds>(speed4)), 9.9492000016256, TOLERANCE); // TODO: Need to validate precision

   speed4 -= speed1;

   ASSERT_NEAR((get_speed_as<Meters, Seconds>(speed4)), -0.050799998374399635, TOLERANCE); // TODO: Need to validate precision
}

// Test Exponent
TEST(Operators, DimensionExponent)
{
   Speed<Meters, Seconds> speed(10.0);

   BaseDimension<UnitExponent<Meters, 3>, UnitExponent<Seconds, -3>> test1 = speed * speed * speed;

   auto test2 = Dimension::Pow<3>(speed);

   ASSERT_TRUE(test1 == test2);
}

// Test Negative
TEST(Operators, DimensionNegative)
{
   Speed<Meters, Seconds> speed = -Speed<Meters, Seconds>(10.0);

   double test = (get_speed_as<Meters, Seconds>(speed));

   ASSERT_NEAR(test, -10.0, TOLERANCE);
}

/*
TEST(Operators, TestSetter) 
{

   using namespace std;

   BaseDimension<UnitExponent<Meters>, UnitExponent<Seconds, -1>> speed(25.0);

   speed.SetVal<tuple<Feet>, tuple<Minutes>>(25.0);

   double res = get_dimension_as<UnitExponent<Meters>, UnitExponent<Seconds, -1>>(speed);

   cout << res << endl;

}
*/

/*
TEST(Operators, TestImplicitCastToAbsoluteQuantity)
{
    using namespace Dimension;

    Temperature<Quantity<Celsius>> temp{10.0};

    BaseDimension<std::tuple<Quantity<Kelvin>, Meters>, std::tuple<Seconds>> obj1;

    auto res1 = obj1 / temp;
    ASSERT_TRUE((std::is_same_v<typename decltype(res1)::NumTuple, std::tuple<Meters>>));

    auto res2 = temp / obj1;
    ASSERT_TRUE((std::is_same_v<typename decltype(res2)::NumTuple, std::tuple<Seconds>>));

    //auto res = obj1 * temp; // Correctly fails to compile
    //auto res = temp * obj1; // Correctly fails to compile

    BaseDimension<std::tuple<Meters>, std::tuple<Quantity<Kelvin>, Seconds>> obj2;

    auto res3 = obj2 * temp;
    ASSERT_TRUE((std::is_same_v<typename decltype(res3)::DenTuple, std::tuple<Seconds>>));

    auto res4 = temp * obj2;
    ASSERT_TRUE((std::is_same_v<typename decltype(res4)::DenTuple, std::tuple<Seconds>>));

    //auto res = obj2 / temp; // Correctly fails to compile
    //auto res = temp / obj2; // Correctly fails to compile

}
*/
