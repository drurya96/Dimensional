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

TEST(TempTests, Test3) {

   length x1 = make_length<meters>(10.0, symbols::sqrt2{}, symbols::pi{});
   EXPECT_NEAR((get_length_as<meters>(x1)), 44.427501395, 0.1);

   length x2 = make_length<meters>(std::ratio<1,4>{}, symbols::pi{});
   EXPECT_NEAR((get_length_as<meters>(x2)), 0.785375, 0.1);

   auto x = x1 * x2;

   static_assert(std::tuple_size_v<typename decltype(x)::symbols> == 2);
   EXPECT_TRUE((std::is_same_v<typename decltype(x)::symbols, std::tuple<symbol_exponent<symbols::sqrt2, 1, 1>, symbol_exponent<symbols::pi, 2, 1>>>));
   EXPECT_TRUE((std::is_same_v<typename decltype(x)::ratio, std::ratio<1,4>>));

   double y = get_area_as<meters>(x);
   EXPECT_NEAR(y, 34.89224890, 0.1);

   length x3 = make_length<meters>(std::ratio<1,2>{}, symbols::pi{});

   //auto x4 = x / x3; 
   length x4 = x / x3;
   static_assert(std::tuple_size_v<typename decltype(x4)::symbols> == 2);
   EXPECT_TRUE((std::is_same_v<typename decltype(x4)::symbols, std::tuple<symbol_exponent<symbols::sqrt2, 1, 1>, symbol_exponent<symbols::pi, 1, 1>>>));
   EXPECT_TRUE((std::is_same_v<typename decltype(x4)::ratio, std::ratio<1,2>>));
   static_assert(std::is_same_v<std::ratio_divide<std::ratio<1,4>, std::ratio<1,2>>, std::ratio<1,2>>);

   EXPECT_NEAR((get_length_as<meters>(x4)), 22.2137506975, 0.1);








   //using Raw = decltype(x / x3);
   //using Raw = decltype(x);

   //static_assert(std::tuple_size_v<typename Raw::units> == 1,
   //            "unit canonicaliser left extra units!");





}

TEST(TempTests, powTest) {

   
   constexpr double a = 8.0;
   constexpr double b = pow_impl(a, 2, 3);

   EXPECT_NEAR(b, 4.0, 0.01);
   

   //constexpr double val = constexpr_root(8.0, 3);
   //EXPECT_NEAR(val, 2.0, 0.01);

}


TEST(TempTests, areaTest) {
   volume<meters> v{10.0};
   length<meters> l{2.0};

   //area a = v / l; // Works...
   area<meters> a = v / l; // Fails...

   static_assert(std::is_same_v<typename decltype(a)::units, std::tuple<unit_exponent<meters, 2, 1>>>);

}








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
 
   



   using coeffs =
      std::tuple<
         symbols::pi,                                             // π¹
         symbol_exponent<symbols::pi, 1, 2>,                      // π¹⁄²
         symbol_exponent<symbols::sqrt2, 3, 1>,                   // √2³
         std::ratio<2,3>                                          // ratio should be ignored
      >;

   using result = build_symbols<std::tuple<>,                      // start empty
                              symbols::pi,
                              symbol_exponent<symbols::pi,1,2>,
                              symbol_exponent<symbols::sqrt2,3,1>,
                              std::ratio<2,3>>::type;

   static_assert(std::tuple_size_v<result> == 2);
   static_assert((std::is_same_v<
      result,
      std::tuple<
         symbol_exponent<symbols::pi, 3, 2>,                      // 1 + 1⁄2
         symbol_exponent<symbols::sqrt2, 3, 1>>>));



         



   using tupA = std::tuple<
      symbol_exponent<symbols::pi,  3, 2>,   // π³⁄²
      symbols::sqrt2                         // √2¹
   >;
   using tupB = std::tuple<
      symbol_exponent<symbols::pi,  1, 2>,   // π¹⁄²
      symbols::sqrt2                         // √2¹
   >;

   /*  π³⁄²·√2¹  *  π¹⁄²·√2¹  ==  π²·√2²  */
   using prod = multiply_symbol_tuples<tupA, tupB>::type;

   static_assert(std::tuple_size_v<prod> == 2); // Should be two
   static_assert(std::is_same_v<
      prod,
      std::tuple<
         symbol_exponent<symbols::pi,   2, 1>,
         symbol_exponent<symbols::sqrt2,2, 1>>>);

   /*  tupA / tupB  ==  π¹·√2⁰  -> single π¹  */
   using quot = divide_symbol_tuples<tupA, tupB>::type;
   static_assert(std::tuple_size_v<quot> == 1);
   static_assert(std::is_same_v<
      quot,
      std::tuple<
         symbol_exponent<symbols::pi, 1, 1>>>);

}
