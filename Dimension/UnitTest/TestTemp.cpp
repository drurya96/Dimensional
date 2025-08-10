#include "DimensionTest.h"
#include <chrono>

using namespace dimension;


static double get_value(angle<radians> a)
{
   return get_angle_as<radians>(a);
}

TEST(TempTests, Test1) {

   length<double, meters> a{5};
   base_dimension_impl<int, unit_exponent<meters>> t{5};

   static_assert(same_unit_representation<std::tuple<unit_exponent<meters>>, typename decltype(t)::units>);

   double b = get_dimension_as<unit_exponent<meters>>(t);
   double c = get_length_as<kilo_meters>(a);

   EXPECT_DOUBLE_EQ(b, 5.0);
   EXPECT_DOUBLE_EQ(c, 0.005);

}


TEST(TempTests, Test2) {

   angle<double, radians> a{5.0};

   double b = get_value(a);

   EXPECT_DOUBLE_EQ(b, 5.0);
}

TEST(TempTests, Test3) {

   length x1 = make_length<meters>(10.0, symbols::sqrt2{}, symbols::pi{});
   EXPECT_NEAR((get_length_as<meters>(x1)), 44.427501395, 0.1);

   length x2 = make_length<meters>(std::ratio<1,4>{}, symbols::pi{});
   EXPECT_NEAR((get_length_as<meters>(x2)), 0.785375, 0.1);

   auto x = x1 * x2;

   //static_assert(std::tuple_size_v<typename decltype(x)::symbols> == 2);
   EXPECT_TRUE((std::is_same_v<typename decltype(x)::symbols, std::tuple<symbol_exponent<symbols::sqrt2, 1, 1>, symbol_exponent<symbols::pi, 2, 1>>>));
   EXPECT_TRUE((std::is_same_v<typename decltype(x)::ratio, std::ratio<1,4>>));

   double y = get_area_as<meters>(x);
   EXPECT_NEAR(y, 34.89224890, 0.1);

   length x3 = make_length<meters>(std::ratio<1,2>{}, symbols::pi{});

   //auto x4 = x / x3; 
   length x4 = x / x3;
   //static_assert(std::tuple_size_v<typename decltype(x4)::symbols> == 2);
   EXPECT_TRUE((std::is_same_v<typename decltype(x4)::symbols, std::tuple<symbol_exponent<symbols::sqrt2, 1, 1>, symbol_exponent<symbols::pi, 1, 1>>>));
   EXPECT_TRUE((std::is_same_v<typename decltype(x4)::ratio, std::ratio<1,2>>));
   static_assert(std::is_same_v<std::ratio_divide<std::ratio<1,4>, std::ratio<1,2>>, std::ratio<1,2>>);

   EXPECT_NEAR((get_length_as<meters>(x4)), 22.2137506975, 0.1);

   force<newtons> z{10.0};

   std::cout << "Value of z: " << z << std::endl;

}

TEST(TempTests, powTest) {
   constexpr double a = 8.0;
   constexpr double b = pow_rational(a, 2, 3);

   EXPECT_NEAR(b, 4.0, 0.01);

   double a2 = 8.0;
   int num = 2;
   int den = 3;
   double b2 = pow_rational(a2, num, den);
   EXPECT_NEAR(b2, 4.0, 0.01);
}

TEST(TempTests, areaTest) {
   volume<meters> v{10.0};
   length<meters> l{2.0};

   area<meters> a = v / l;

   static_assert(std::is_same_v<typename decltype(a)::units, std::tuple<unit_exponent<meters, 2, 1>>>);

}






/*
TEST(TempTests, symbolTest) {

   using empty   = std::tuple<>;
   
   using step1   = add_symbol<empty, symbols::pi, std::ratio<1>>::type;
   static_assert(std::tuple_size_v<step1> == 1);
   static_assert(std::is_same_v<
      std::tuple_element_t<0, step1>,
      symbol_exponent<symbols::pi, 1, 1>>);

   // merge π¹ + π¹ → π²
   using step2   = add_symbol<step1, symbols::pi, std::ratio<1>>::type;
   static_assert(std::is_same_v<
      std::tuple_element_t<0, step2>,
      symbol_exponent<symbols::pi, 2, 1>>);

   // merge π² + π⁻² → empty tuple
   using step3   = add_symbol<step2, symbols::pi, std::ratio<-2>>::type;
   static_assert(std::tuple_size_v<step3> == 0);

   using tupA = std::tuple<
      symbol_exponent<symbols::pi,  3, 2>,   // π³⁄²
      symbols::sqrt2                         // √2¹
   >;
   using tupB = std::tuple<
      symbol_exponent<symbols::pi,  1, 2>,   // π¹⁄²
      symbols::sqrt2                         // √2¹
   >;

   using prod = multiply_symbol_tuples<tupA, tupB>::type;

   static_assert(std::tuple_size_v<prod> == 2); // Should be two
   static_assert(std::is_same_v<
      prod,
      std::tuple<
         symbol_exponent<symbols::pi,   2, 1>,
         symbol_exponent<symbols::sqrt2,2, 1>>>);

   using quot = divide_symbol_tuples<tupA, tupB>::type;
   static_assert(std::tuple_size_v<quot> == 1);
   static_assert(std::is_same_v<
      quot,
      std::tuple<
         symbol_exponent<symbols::pi, 1, 1>>>);

}
*/

TEST(TempTests, quickTest) {

   base_dimension_impl<double, unit_exponent<meters>, symbol_exponent<symbols::pi, 1, 1>> test1{10.0};

   //using t = typename decltype(test1)::ts_split;
   //using coeffs = typename decltype(test1)::coeffs;
   //static_assert(std::is_same_v<typename decltype(test1)::symbols, std::tuple<symbol_exponent<symbols::pi, 1, 1>>>);
   //static_assert(std::is_same_v<typename decltype(test1)::symbols, std::tuple<symbols::pi>>);
   //static_assert(std::tuple_size_v<typename decltype(test1)::symbols> == 1);
   //static_assert(std::tuple_size_v<coeffs> == 1);

   //static_assert(is_symbol<std::tuple_element_t<0, coeffs>>);
   //static_assert(std::is_same_v<std::tuple_element_t<0, coeffs>, symbols::pi>);

   //using part = detail::coefficient_impl::partition_coeffs<coeffs>;
   
   //using s = typename part::symbols;
   //using r =  typename part::ratios;

   //static_assert(std::tuple_size_v<s> == 1);
   //static_assert(std::tuple_size_v<r> == 0);

   //static_assert(std::tuple_size_v<detail::collect_symbol_exponents_t<symbols::pi>> == 1);




   //using ratio_list = ratio_multiply_tuple_t<typename part::ratios>;
   //using symbol_tuple = collapse_symbol_exponents<typename part::symbols>;

   //using x = ratio_multiply_tuple_t<typename part::ratios>;
   //using y = detail::collapse_symbol_exponents_t<typename part::symbols>;

   //static_assert(std::tuple_size_v<x> == 0);
   //static_assert(std::tuple_size_v<y> == 1);






   EXPECT_NEAR(get_length_as<meters>(test1), 10.0*std::numbers::pi, 0.001);

   // Intentionally fails to compile
   //base_dimension_impl<double, unit_exponent<meters>> test2{10.0, symbols::pi{}};
/*
   using a = std::tuple<symbol_exponent<symbols::pi, 1, 1>, symbol_exponent<symbols::e, 2, 1>>;
   using b = std::tuple<symbol_exponent<symbols::pi, 1, 1>, symbol_exponent<symbols::e, 1, 1>>;

   using res = typename multiply_symbol_tuples<a, b>::type;

   static_assert(std::tuple_size_v<res> == 2);
   using res0 = typename std::tuple_element_t<0, res>;
   using res1 = typename std::tuple_element_t<1, res>;

   std::cout << "First item value: " << symbol_exponent_value<res0>() << "(" << res0::symbol::value << ", " << res0::exponent::num << ", " << res0::exponent::den << ")" << std::endl;
   std::cout << "Second item value: " << symbol_exponent_value<res1>() << "(" << res1::symbol::value << ", " << res1::exponent::num << ", " << res1::exponent::den << ")" << std::endl;


   static_assert(std::is_same_v<res, std::tuple<symbol_exponent<symbols::e, 3, 1>, symbol_exponent<symbols::pi, 2, 1>>>);








   double val = multiply_symbol_exponent_values<std::tuple<>>();
   std::cout << "value: " << val << std::endl;
*/


   //base_dimension_impl<double, unit_exponent<meters>, symbol




}
