#include "DimensionTest.h"

using namespace dimension;
using namespace std;

TEST(CastOperatorTests, Castspeed) {

   base_dimension<unit_exponent<meters>, unit_exponent<seconds, -1>> testspeed(10.0);

   speed<meters, seconds> myspeed1 = testspeed;
   speed myspeed2 = testspeed; // Uses template deduction guide
   speed<feet, minutes> myspeed3 = testspeed; // Cast to different unit

   ASSERT_NEAR((get_speed_as<meters, seconds>(myspeed1)), 10.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, seconds>(myspeed2)), 10.0, TOLERANCE);
   ASSERT_NEAR((get_speed_as<meters, seconds>(myspeed3)), 10.0, TOLERANCE);

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // base_dimension<tuple<meters, seconds>, tuple<>> testNotspeed(10.0);
   // speed notspeed = testNotspeed;
}

TEST(CastOperatorTests, Castlength) {

   base_dimension<unit_exponent<meters>> testlength(10.0);

   length<meters> mylength1 = testlength;
   length mylength2 = testlength; // Uses template deduction guide
   length<feet> mylength3 = testlength; // Cast to different unit

   ASSERT_NEAR(get_length_as<meters>(mylength1), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<meters>(mylength2), 10.0, TOLERANCE);
   ASSERT_NEAR(get_length_as<meters>(mylength3), 10.0, TOLERANCE);

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // base_dimension<tuple<seconds>, tuple<>> testNotlength(10.0);
   // length notlength = testNotlength;
}

TEST(CastOperatorTests, Casttimespan) {

   base_dimension<unit_exponent<seconds>> testtimespan(10.0);

   timespan<seconds> mytimespan1 = testtimespan;
   timespan mytimespan2 = testtimespan; // Uses template deduction guide
   timespan<minutes> mytimespan3 = testtimespan; // Cast to different unit

   ASSERT_NEAR(get_timespan_as<seconds>(mytimespan1), 10.0, TOLERANCE);
   ASSERT_NEAR(get_timespan_as<seconds>(mytimespan2), 10.0, TOLERANCE);
   ASSERT_NEAR(get_timespan_as<seconds>(mytimespan3), 10.0, TOLERANCE);

   // TODO: I need to find a way to ensure the following DOES NOT compile
   // base_dimension<tuple<meters>, tuple<>> testNottimespan(10.0);
   // timespan nottimespan = testNottimespan;
}

TEST(CastOperatorTests, CastScalar) {

   base_dimension<unit_exponent<seconds>> testtimespan(10.0);

   base_dimension<> scalar = testtimespan / timespan<seconds>(5.0);

   ASSERT_NEAR(scalar, 2.0, TOLERANCE);
}
