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







































struct in_rain {};
struct in_snow {};

constexpr double EPS = 1e-9;


TEST(Labels, NoCancelWithUnlabeled)
{
  // 2 in[rain] * (3 in^-1)  -> should NOT cancel (labels differ)
  using UE_labeled   = unit_exponent<inches, 1, 1, in_rain>;
  using UE_unlabeled = unit_exponent<inches, -1, 1>;

  base_dimension<UE_labeled>   a(2.0);
  base_dimension<UE_unlabeled> b(3.0);

  auto prod = a * b;

  // Expect the result still has both factors: in[rain]^1 * in^-1
  // Extract with both exponents present
  double v = get_dimension_as<UE_labeled, UE_unlabeled>(prod);
  EXPECT_NEAR(v, 6.0, EPS);
}

TEST(Labels, CancelWithSameLabel)
{
  // 2 in[rain] * (3 in[rain]^-1)  -> dimensionless 6
  using UE_rain     = unit_exponent<inches, 1, 1, in_rain>;
  using UE_rain_inv = unit_exponent<inches, -1, 1, in_rain>;

  base_dimension<UE_rain>     a(2.0);
  base_dimension<UE_rain_inv> b(3.0);

  auto ratio = a * b;

  // If labeled cancellation works, this is dimensionless and extractable via empty <>
  EXPECT_NEAR(get_dimension_as<>(ratio), 6.0, EPS);
}

TEST(Labels, PreserveLabelThroughDivision)
{
  // (2 in[rain]) / (4 ft^2)  -> in[rain] / ft^2 with value 0.5
  using UE_rain = unit_exponent<inches, 1, 1, in_rain>;
  using UE_ft2  = unit_exponent<feet,   2, 1>;

  base_dimension<UE_rain> depth(2.0);
  base_dimension<UE_ft2>  area (4.0);

  auto rate = depth / area;

  // Extract in the same units/exponents, label must still be on the inches factor
  double v = get_dimension_as<UE_rain, unit_exponent<feet, -2>>(rate);
  EXPECT_NEAR(v, 0.5, EPS);
}

TEST(Labels, ConversionIgnoresLabelButPreservesIt)
{
  // Convert (0.5 in[rain] / ft^2) to (m[rain] / m^2)
  using UE_rain_in = unit_exponent<inches, 1, 1, in_rain>;
  using UE_ft2     = unit_exponent<feet,   2, 1>;

  base_dimension<UE_rain_in> depth(2.0);
  base_dimension<UE_ft2>     area (4.0);
  auto rate = depth / area; // 0.5 in[rain] / ft^2

  // expected numeric: 0.5 * (in→m) / (ft→m)^2
  double expected = 0.5 * 0.0254 / (0.3048 * 0.3048);

  double got = get_dimension_as<
                 unit_exponent<meters, 1, 1, in_rain>,
                 unit_exponent<meters, -2>
               >(rate);
  EXPECT_NEAR(got, expected, 1e-12);
}

TEST(Labels, AdditionRequiresMatchingLabel)
{
  // Same label: add OK
  using E1 = unit_exponent<inches, 1, 1, in_rain>;
  using E2 = unit_exponent<inches, 1, 1, in_rain>;

  base_dimension<E1> a(2.0);
  base_dimension<E2> b(3.0);

  auto sum = a + b;
  EXPECT_NEAR((get_dimension_as<unit_exponent<inches,1,1,in_rain>>(sum)), 5.0, EPS);

  // Different label: if your library enforces it at compile time, the following
  // should be ill-formed. Keep commented if it’s a hard error.
  // using F1 = unit_exponent<inches, 1, 1, in_rain>;
  // using F2 = unit_exponent<inches, 1, 1, in_snow>;
  // base_dimension<F1> x(1.0);
  // base_dimension<F2> y(1.5);
  // auto bad = x + y; // expect compile-time error (mismatched exponents)
}

TEST(Labels, LabeledVsUnlabeledEqualityKey)
{
  // Sanity on the key: labeled inches != unlabeled inches; labeled inches == same labeled inches
  using A = unit_exponent<inches, 1, 1>;
  using B = unit_exponent<inches, 1, 1, in_rain>;
  using C = unit_exponent<inches, 1, 1, in_rain>;

  static_assert(!std::is_same_v<typename A::label, typename B::label>, "void vs in_rain must differ");
  static_assert( std::is_same_v<typename B::label, typename C::label>,  "same label types must match");
}

TEST(Labels, MixedAlgebraKeepsLabelOnTheRightFactor)
{
  // (in[rain] * m) / (ft * m) -> in[rain] / ft (the 'm' cancels, label stays on inches)
  using E1 = unit_exponent<inches, 1, 1, in_rain>;
  using E2 = unit_exponent<meters, 1, 1>;
  using F1 = unit_exponent<feet,   1, 1>;
  using F2 = unit_exponent<meters, 1, 1>;

  base_dimension<E1, E2> num(3.0);  // 3 * in[rain] * m
  base_dimension<F1, F2> den(2.0);  // 2 * ft * m

  auto r = num / den;

  double got = get_dimension_as<
                 unit_exponent<inches, 1, 1, in_rain>,
                 unit_exponent<feet, -1>
               >(r);
  // numeric factor 3/2; conversions (in→in, ft→ft) do nothing in this extraction
  EXPECT_NEAR(got, 1.5, EPS);
}








// SOME (OR ALL?) OF BELOW ARE SUPPOSED TO FAIL!!!


//constexpr double EPS = 1e-12;
/*
//------------------------------------------------------------------------------
// 1) Labeled * unlabeled inches should NOT collapse to unlabeled in^2
//------------------------------------------------------------------------------
TEST(LabelsNegative, ProductNotUnlabeledSquare)
{
   using E_labeled   = unit_exponent<inches, 1, 1, in_rain>;
   using E_unlabeled = unit_exponent<inches, 1, 1>;

   base_dimension<E_labeled>   a(2.0);
   base_dimension<E_unlabeled> b(3.0);

   auto prod = a * b; // in[rain]^1 * in^1

   using Res = decltype(prod);

   static_assert(std::tuple_size_v<Res::units> == 2); // Should fail, actually passes.

   std::cout << prod << std::endl;

   //double v = get_dimension_as<unit_exponent<inches, 2, 1>>(prod);

   //EXPECT_NEAR(v, 6.0, EPS);
}
*/
TEST(LabelsNegative, ProductNotUnlabeledSquare2)
{

   using exp1 = unit_exponent<inches, 1, 1, in_rain>;
   using exp2 = unit_exponent<inches, 1, 1>;

   using conc = std::tuple<exp1, exp2>;

   using Res1 = typename collapse_units<conc>::type; // tuple of resulting unit exponents 
   using Res3 = typename collapse_units<conc>::combined; // tuple of resulting unit exponents 
   using Res2 = typename unit_decomposition<conc>::units; // tuple of resulting unit exponents 

   static_assert(std::tuple_size_v<Res1> == 2); // Bad... should be 2
   static_assert(std::tuple_size_v<Res2> == 2); // Good!!
   static_assert(std::tuple_size_v<Res3> == 2); // Bad... should be 2


   using Res2_0 = std::tuple_element_t<0, Res2>;
   using Res2_1 = std::tuple_element_t<1, Res2>;


   static_assert(std::is_same_v<typename Res2_0::label, in_rain>); // BAD! This should be in_rain!!!
   static_assert(std::is_same_v<typename Res2_1::label, void>);

   //static_assert(std::is_same_v<Res2_0, unit_exponent<inches, 1, 1, in_rain>>);
   //static_assert(std::is_same_v<Res2_1, unit_exponent<inches, 1, 1, void>>);

}