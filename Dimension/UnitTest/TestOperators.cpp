#include "DimensionTest.h"

using namespace dimension;
using namespace std;

TEST(Operators, Comparisons) {

   speed<meters, seconds> myspeed1(10.0); // 10.0 m/s == 1968.504 f/m
   speed<feet, minutes> myspeed2(20.0); // 20.0 f/m == 0.1016 m/s
   speed<feet, minutes> myspeed3(1968.5);
   speed<meters, seconds> myspeedZero1(0.0);
   speed<feet, minutes> myspeedZero2(0.0);

   std::cout << get_speed_as<feet, minutes>(myspeed1) << std::endl;
   std::cout << get_speed_as<meters, seconds>(myspeed3) << std::endl;

   // The ASSERT_GE and ASSERT_LE syntax would work here
   //   but since this is a test of the operators, they are written explicitly
   
   ASSERT_TRUE(myspeed1 > myspeed2);
   
   ASSERT_TRUE(myspeed2 < myspeed1);
   //ASSERT_TRUE(myspeed1.NearlyEqual(myspeed3, 0.001)); TODO: Reconsider NearlyEqual
   ASSERT_FALSE(myspeedZero2 != myspeedZero1);
   ASSERT_TRUE(myspeedZero1 == myspeedZero2);
   ASSERT_TRUE(myspeed1 != myspeed2);
   ASSERT_FALSE(myspeed1 == myspeed2);
   ASSERT_TRUE(myspeed1 >= myspeed2 && myspeed1 >= myspeed3);
   ASSERT_TRUE(myspeed2 <= myspeed1 && myspeed3 <= myspeed1); 
   
}



TEST(Operators, DimensionMultiplication) {

   length<meters> mylength(10.0);
   timespan<seconds> mytimespan(5.0);

   // The following operator should fail to compile, not sure how to unit test a compile timespan failure though
   // speed myspeed = mylength * mytimespan;
   
   auto myTest = mylength * mytimespan;

   ASSERT_NEAR((get_dimension_as<unit_exponent<meters>, unit_exponent<seconds>>(myTest)), 50.0, TOLERANCE);

   ASSERT_NEAR((get_dimension_as<unit_exponent<feet>, unit_exponent<minutes>>(myTest)), 2.73403333333333333, TOLERANCE);

   ASSERT_TRUE((is_same_v<decltype(myTest), base_dimension<unit_exponent<meters>, unit_exponent<seconds>>>));
}


TEST(Operators, DimensionDivision) {

   length<meters> mylength(20.0);
   timespan<seconds> mytimespan(2.0);

   // The following operator should fail to compile, not sure how to unit test a compile timespan failure though
   //base_dimension<lengthUnit, timespanUnit> = mylength / mytimespan;

   auto myTest = mylength / mytimespan;

   ASSERT_NEAR((get_dimension_as<unit_exponent<meters>, unit_exponent<seconds, -1>>(myTest)), 10.0, TOLERANCE);
   ASSERT_NEAR((get_dimension_as<unit_exponent<feet>, unit_exponent<minutes, -1>>(myTest)), 1968.504, TOLERANCE);

   ASSERT_TRUE((is_same_v<decltype(myTest), base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>>>));
   //ASSERT_NO_THROW(speed myspeed = myTest); // TODO: This is really a test of the cast operator
}


// Test Scalar multiplation
TEST(Operators, ScalarMultiplication)
{
   speed<meters, seconds> myspeed(5.0);

   myspeed = myspeed * 2;
   
   ASSERT_NEAR((get_speed_as<meters, seconds>(myspeed)), 10.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<feet, seconds>(myspeed)), 32.8084, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, minutes>(myspeed)), 600.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<feet, minutes>(myspeed)), 1968.504, TOLERANCE);
   
   myspeed *= 2;
   
   ASSERT_NEAR((get_speed_as<meters, seconds>(myspeed)), 20.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<feet, seconds>(myspeed)), 65.6168, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, minutes>(myspeed)), 1200.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<feet, minutes>(myspeed)), 3937.008, TOLERANCE);
   
}

// Test Scalar division
TEST(Operators, ScalarDivision)
{
   speed<meters, seconds> myspeed(20.0);

   myspeed = myspeed / 2;

   ASSERT_NEAR((get_speed_as<meters, seconds>(myspeed)), 10.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<feet, seconds>(myspeed)), 32.8084, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, minutes>(myspeed)), 600.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<feet, minutes>(myspeed)), 1968.504, TOLERANCE);

   myspeed /= 2;

   ASSERT_NEAR((get_speed_as<meters, seconds>(myspeed)), 5.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<feet, seconds>(myspeed)), 16.4042, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, minutes>(myspeed)), 300.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<feet, minutes>(myspeed)), 984.252, TOLERANCE);
}

// Test addition
TEST(Operators, DimensionAddition)
{
   speed<meters, seconds> speed1(10.0);
   speed<feet, minutes> speed2(10.0);

   auto speed3 = speed1 + speed2;
   speed speed4 = speed1 + speed2;

   ASSERT_TRUE((is_same<decltype(speed3), base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>>>::value));

   ASSERT_NEAR((get_speed_as<meters, seconds>(speed4)), 10.0507999983744, TOLERANCE); // TODO: Need to validate precision

   speed4 += speed1;
   ASSERT_NEAR((get_speed_as<meters, seconds>(speed4)), 20.050799998374401, TOLERANCE); // TODO: Need to validate precision
}

// Test subtraction
TEST(Operators, DimensionSubtraction)
{
   speed<meters, seconds> speed1(10.0);
   speed<feet, minutes> speed2(10.0);

   auto speed3 = speed1 - speed2;
   speed speed4 = speed1 - speed2;

   ASSERT_TRUE((is_same<decltype(speed3), base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>>>::value));

   ASSERT_NEAR((get_speed_as<meters, seconds>(speed4)), 9.9492000016256, TOLERANCE); // TODO: Need to validate precision

   speed4 -= speed1;

   ASSERT_NEAR((get_speed_as<meters, seconds>(speed4)), -0.050799998374399635, TOLERANCE); // TODO: Need to validate precision
}

// Test Exponent
TEST(Operators, DimensionExponent)
{
   speed<meters, seconds> speed(10.0);

   base_dimension<unit_exponent<meters, 3>, unit_exponent<seconds, -3>> test1 = speed * speed * speed;

   auto test2 = dimension::Pow<3>(speed);

   ASSERT_TRUE(test1 == test2);
}

// Test Negative
TEST(Operators, DimensionNegative)
{
   speed<meters, seconds> speed1 = -speed<meters, seconds>(10.0);

   double test = (get_speed_as<meters, seconds>(speed1));

   ASSERT_NEAR(test, -10.0, TOLERANCE);
}

/*
TEST(Operators, TestSetter) 
{

   using namespace std;

   base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>> speed(25.0);

   speed.SetVal<tuple<feet>, tuple<minutes>>(25.0);

   double res = get_dimension_as<unit_exponent<meters>, unit_exponent<seconds, -1>>(speed);

   cout << res << endl;

}
*/

/*
TEST(Operators, TestImplicitCastToAbsoluteQuantity)
{
    using namespace dimension;

    temperature<Quantity<Celsius>> temp{10.0};

    base_dimension<std::tuple<Quantity<Kelvin>, meters>, std::tuple<seconds>> obj1;

    auto res1 = obj1 / temp;
    ASSERT_TRUE((std::is_same_v<typename decltype(res1)::NumTuple, std::tuple<meters>>));

    auto res2 = temp / obj1;
    ASSERT_TRUE((std::is_same_v<typename decltype(res2)::NumTuple, std::tuple<seconds>>));

    //auto res = obj1 * temp; // Correctly fails to compile
    //auto res = temp * obj1; // Correctly fails to compile

    base_dimension<std::tuple<meters>, std::tuple<Quantity<Kelvin>, seconds>> obj2;

    auto res3 = obj2 * temp;
    ASSERT_TRUE((std::is_same_v<typename decltype(res3)::DenTuple, std::tuple<seconds>>));

    auto res4 = temp * obj2;
    ASSERT_TRUE((std::is_same_v<typename decltype(res4)::DenTuple, std::tuple<seconds>>));

    //auto res = obj2 / temp; // Correctly fails to compile
    //auto res = temp / obj2; // Correctly fails to compile

}
*/
