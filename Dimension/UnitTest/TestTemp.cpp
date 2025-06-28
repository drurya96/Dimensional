#include "DimensionTest.h"
#include <chrono>

using namespace dimension;

double get_value(angle<radians> a)
{
   return get_angle_as<radians>(a);
}


TEST(TempTests, Test1) {

   length<double, meters> a{5};
   base_dimension_impl<int, unit_exponent<meters>> t{5};
   //double b = t.get<unit_exponent<meters>>();
   //double b = t.get<feet>();

   static_assert(same_units<std::tuple<unit_exponent<meters>>, typename decltype(t)::units>);

   //double b = get_dimension_as(a);
   double b = get_dimension_as<unit_exponent<meters>>(t);
   //double b = get_length_as<meters>(a);
   double c = get_length_as<kilo_meters>(a);

   EXPECT_DOUBLE_EQ(b, 5.0);
   EXPECT_DOUBLE_EQ(c, 0.005);

}

TEST(TempTests, Test2) {

   angle<double, radians> a{5.0};

   double b = get_value(a);

   EXPECT_DOUBLE_EQ(b, 5.0);
}
