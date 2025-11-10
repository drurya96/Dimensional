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
/*
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
  */

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




/*

TEST(TempTests, testBuiltinRatio) {
   acceleration<meters, seconds> a{10.0};
   timespan<minutes> t{2.0};

   auto raw = a * t; // This should just be a speed, simplification has not yet occured
   speed s = raw; // Ensuring can be cast to speed with type deduction, simplification occurs. Should be 1200 m/s

   using sType = decltype(s); // Should be speed<meters, seconds>

   using raw_units = typename decltype(raw)::units; // Should be std::tuple<unit_exponent<meters>, unit_exponent<seconds, -1>, unit_exponent<minutes, -1>

   static_assert(std::tuple_size_v<raw_units> == 3);

   using simp = simplification::SimplifiedDimension<raw_units>;

   // I don't remember what this was supposed to be testing...
   //static_assert(std::is_same_v<typename simp::after_conversion::a, std::ratio<1>>);
   //static_assert(std::is_same_v<typename simp::after_conversion::b, std::ratio<1>>);
   static_assert(std::is_same_v<typename simp::after_conversion::ratio, std::ratio<60>>); // This passes.. I think this is right? I've lost the thread a bit

   static_assert(std::is_same_v<typename simp::ratio, std::ratio<60>>); // This seems right I think? I would have thought negative ratio though... TBD

   static_assert(std::is_same_v<typename sType::ratio, std::ratio<1>>); // I think this is right. The deduced type should resolve the ratio into the scalar to avoid type explosion



   EXPECT_EQ((get_speed_as<meters, seconds>(s)), 1200); // Good.



   using conv = simplification::Convert_All_Units<unit_exponent<feet>, std::tuple<unit_exponent<meters>>>; // Order seems to matter... I bet there's an issue dealing with "no conversion found"... Future self - of course order matters? It would be inverted.. right?


   static_assert(std::is_same_v<typename conv::TEST, std::ratio<1250, 381>>); // Checks out, going from meters to feet. Above, feet is the target unit.
   static_assert(std::is_same_v<typename conv::ratio, std::ratio<1250, 381>>); // Checks out, going from meters to feet. Above, feet is the target unit.



   static_assert(std::is_same_v<simplification::conversion_ratio_t<meters, feet>, std::ratio<1250, 381>>); // GOOD, this is a start.





}


TEST(TempTests, testRaiseRatio){

   using r = std::ratio<1,2>;

   using n = raise_ratio_t<r, -2>;

   static_assert(std::is_same_v<n, std::ratio<4, 1>>);


}


TEST(TempTests, testSimplifiedUnits){

   // Before getting to the real test, lets see what behavior we get when we use unit_decomposition on each of fundamental and derived units
   // This should help work out the problem. Not necessarily answer, but help.

   using res = typename unit_decomposition<std::tuple<unit_exponent<knots>>>::units;

   static_assert(std::is_same_v<res, res>); // Just to make sure res is used, not a real test

   // This is correct. Now we can move on to the sare_simplified_units
   static_assert(std::is_same_v<res, std::tuple<unit_exponent<nautical_miles>, unit_exponent<hours, -1>>>);


   static_assert(std::is_same_v<typename dimension::details::get_factor_t<meters, feet>::ratio, std::ratio<1250, 381>>);
   static_assert(std::is_same_v<typename dimension::details::get_factor_t<feet, meters>::ratio, std::ratio<381, 1250>>);
   static_assert(std::is_same_v<typename dimension::details::get_factor_t<feet, feet>::ratio, std::ratio<1>>);
   static_assert(std::is_same_v<typename dimension::details::get_factor_t<feet, miles>::ratio, std::ratio<1, 5280>>);

}

template<class T, class U>
constexpr bool same_v = std::is_same_v<T, U>;

// Convenience alias to the real raiser you implemented:
template<class SE, class P>
using raise_symbol_exponent_t =
typename dimension::detail::symbol_utils_impl::raise_symbol_exponent<SE, P>::type;

TEST(TempTests, testRaiseFactor){

   // -----------------------------
   // ratio_exponent tests
   // -----------------------------

   // Base: (3/5)^(2)  and  (7/3)^(-1)
   using RE1 = ratio_exponent<std::ratio<3,5>,  2>;
   using RE2 = ratio_exponent<std::ratio<7,3>, -1>;

   // 1) Integer power: P = 3  => exponents multiply by 3
   using RE1_P3 = typename raise_ratio_exponent<RE1, std::ratio<3>>::type;
   using RE2_P3 = typename raise_ratio_exponent<RE2, std::ratio<3>>::type;

   static_assert(same_v<
   RE1_P3,
   ratio_exponent<std::ratio<3,5>, 6>
   >, "ratio_exponent × 3 failed");

   static_assert(same_v<
   RE2_P3,
   ratio_exponent<std::ratio<7,3>, -3>
   >, "ratio_exponent × 3 (negative base exp) failed");

   // 2) Fractional power: P = 1/2
   using RE1_P12 = typename raise_ratio_exponent<RE1, std::ratio<1,2>>::type;
   using RE2_P12 = typename raise_ratio_exponent<RE2, std::ratio<1,2>>::type;

   static_assert(same_v<
   RE1_P12,
   ratio_exponent<std::ratio<3,5>, 1>     // 2 * 1/2 = 1
   >, "ratio_exponent × 1/2 (even) failed");

   static_assert(same_v<
   RE2_P12,
   ratio_exponent<std::ratio<7,3>, -1, 2> // -1 * 1/2
   >, "ratio_exponent × 1/2 (odd) failed");

   // 3) Negative integer power: P = -2
   using RE1_N2 = typename raise_ratio_exponent<RE1, std::ratio<-2>>::type;
   static_assert(same_v<
   RE1_N2,
   ratio_exponent<std::ratio<3,5>, -4>
   >, "ratio_exponent × -2 failed");

   // 4) Zero power: P = 0  (exponent becomes 0)
   using RE1_0 = typename raise_ratio_exponent<RE1, std::ratio<0>>::type;
   static_assert(same_v<
   RE1_0,
   ratio_exponent<std::ratio<3,5>, 0>
   >, "ratio_exponent × 0 failed");

   // ------------------------------------
   // symbol_exponent tests (uses pi)
   // ------------------------------------

   // Base cases
   using SEa = dimension::symbol_exponent<symbols::pi,  3, 1>;  // pi^(3)
   using SEb = dimension::symbol_exponent<symbols::pi, -2, 1>;  // pi^(-2)

   // 1) Integer power: ×4
   using SEa_P4 = raise_symbol_exponent_t<SEa, std::ratio<4>>;
   using SEb_P4 = raise_symbol_exponent_t<SEb, std::ratio<4>>;

   static_assert(same_v<
   SEa_P4,
   dimension::symbol_exponent<symbols::pi, 12, 1>   // 3 * 4
   >);
   static_assert(same_v<
   SEb_P4,
   dimension::symbol_exponent<symbols::pi, -8, 1>   // -2 * 4
   >);

   // 2) Fractional power that reduces to integer: ×(5/3)
   using SEa_P53 = raise_symbol_exponent_t<SEa, std::ratio<5,3>>;
   static_assert(same_v<
   SEa_P53,
   dimension::symbol_exponent<symbols::pi, 5, 1>    // 3 * 5/3 = 5
   >);

   // 3) Fractional power that stays fractional: ×(1/2)
   using SEb_P12 = raise_symbol_exponent_t<SEb, std::ratio<1,2>>;
   static_assert(same_v<
   SEb_P12,
   dimension::symbol_exponent<symbols::pi, -1, 1>   // -2 * 1/2 = -1
   >);

   // 4) Zero power
   using SEa_P0 = raise_symbol_exponent_t<SEa, std::ratio<0>>;
   static_assert(same_v<
   SEa_P0,
   dimension::symbol_exponent<symbols::pi, 0, 1>
   >);

   // -------------------------
   // Test 1: integer power (P = 3)
   // - base ratio 2 → 8
   // - exponents multiply by 3
   // -------------------------
   using F1 = factor_t<
      std::ratio<2,1>,
      ratio_exponent<std::ratio<3,5>,  2>,
      ratio_exponent<std::ratio<7,3>, -1>
   >;

   using R1 = typename raise_factor<F1, std::ratio<3>>::type;

   using R1_expected = factor_t<
      std::ratio<8,1>, // 2^3
      ratio_exponent<std::ratio<3,5>,  6>,  // 2 * 3
      ratio_exponent<std::ratio<7,3>, -3>   // -1 * 3
   >;

   static_assert(same_v<R1, R1_expected>, "raise_factor integer power failed");

   // -------------------------
   // Test 2: fractional power (P = 1/2)
   // - base ratio becomes 1
   // - exponents multiply by 1/2
   // - new ratio_exponent<BaseRatio, 1, 2> appended
   // -------------------------
   using R2 = typename raise_factor<F1, std::ratio<1,2>>::type;

   using R2_expected = factor_t<
      std::ratio<1,1>,
      ratio_exponent<std::ratio<3,5>, 1>,        // 2 * 1/2 = 1
      ratio_exponent<std::ratio<7,3>, -1, 2>,    // -1 * 1/2
      ratio_exponent<std::ratio<2,1>,  1, 2>     // carried fractional power of base
   >;

   static_assert(same_v<R2, R2_expected>, "raise_factor fractional power failed");

   // -------------------------
   // Test 3: negative integer power (P = -2)
   // - base ratio 2 → 1/4
   // - exponents multiply by -2
   // -------------------------
   using R3 = typename raise_factor<F1, std::ratio<-2>>::type;

   using R3_expected = factor_t<
      std::ratio<1,4>, // 2^-2
      ratio_exponent<std::ratio<3,5>, -4>, // 2 * -2
      ratio_exponent<std::ratio<7,3>,  2>  // -1 * -2
   >;

   static_assert(same_v<R3, R3_expected>, "raise_factor negative integer power failed");

   // 0) Identity & empty cases
   using F_empty = factor_t<std::ratio<1>>;
   static_assert(std::is_same_v<typename raise_factor<F_empty, std::ratio<1>>::type,  F_empty>);
   static_assert(std::is_same_v<typename raise_factor<F_empty, std::ratio<0>>::type, F_empty>);

   // 1) Base ratio with zero power → 1, no carried exponent
   using F_base = factor_t<std::ratio<5,2>>;
   using F_base_P0 = typename raise_factor<F_base, std::ratio<0>>::type;
   static_assert(std::is_same_v<F_base_P0, factor_t<std::ratio<1>>>);

   // 2) Power = 1 leaves structure (but normalizes tuple order you define)
   using F_mix = factor_t<
   std::ratio<3,2>,
   ratio_exponent<std::ratio<9,4>, 2>,
   ratio_exponent<std::ratio<2,3>, -3>
   >;
   using F_mix_P1 = typename raise_factor<F_mix, std::ratio<1>>::type;
   static_assert(std::is_same_v<F_mix_P1, F_mix>);

   // 3) cv/ref noise in input pack is stripped by your filter utilities
   using F_cvref = factor_t<
   std::ratio<2>,
   const ratio_exponent<std::ratio<3,5>, 2>&,
   volatile ratio_exponent<std::ratio<7,3>, -1>
   >;
   using F_cvref_P2 = typename raise_factor<F_cvref, std::ratio<2>>::type;
   using F_cvref_P2_exp = factor_t<
   std::ratio<4>,
   ratio_exponent<std::ratio<3,5>, 4>,
   ratio_exponent<std::ratio<7,3>, -2>
   >;
   static_assert(std::is_same_v<F_cvref_P2, F_cvref_P2_exp>);

   // 4) Symbols participate with fractional power carry
   using SFa = factor_t<
   std::ratio<3,2>,
   dimension::symbol_exponent<symbols::pi, 3, 1>,
   ratio_exponent<std::ratio<5,4>, 2>
   >;
   using SFa_P3_2 = typename raise_factor<SFa, std::ratio<3,2>>::type;
   using SFa_P3_2_exp = factor_t<
   std::ratio<1>,
   dimension::symbol_exponent<symbols::pi, 9, 2>,
   ratio_exponent<std::ratio<5,4>, 3>,
   ratio_exponent<std::ratio<3,2>, 3, 2>
   >;
   static_assert(std::is_same_v<SFa_P3_2, SFa_P3_2_exp>);

   // 5) Negative fractional power carry
   using NF = factor_t<std::ratio<3,2>>;
   using NF_Pneg = typename raise_factor<NF, std::ratio<-1,2>>::type;
   using NF_Pneg_exp = factor_t<std::ratio<1>, ratio_exponent<std::ratio<3,2>, -1, 2>>;
   static_assert(std::is_same_v<NF_Pneg, NF_Pneg_exp>);

   // 6) Zero exponents in tuples are preserved by raise_* then collapsed only where you explicitly do so
   using ZR = factor_t<std::ratio<2>, ratio_exponent<std::ratio<9,4>, 0>>;
   using ZR_P3 = typename raise_factor<ZR, std::ratio<3>>::type;
   using ZR_P3_exp = factor_t<
      std::ratio<8,1>,                              // be explicit
      ratio_exponent<std::ratio<9,4>, 0>
   >;
   static_assert(std::is_same_v<ZR_P3, ZR_P3_exp>);

}



// element-wise equality (order-sensitive)
template<class A, class B, std::size_t... I>
constexpr bool tuple_same_impl(std::index_sequence<I...>) {
  return (std::is_same_v<std::tuple_element_t<I, A>, std::tuple_element_t<I, B>> && ...);
}
template<class A, class B>
constexpr bool tuple_same_v =
  (std::tuple_size_v<A> == std::tuple_size_v<B>) &&
  tuple_same_impl<A, B>(std::make_index_sequence<std::tuple_size_v<A>>{});

// contains
template<class T, class Tuple> struct tuple_contains;
template<class T> struct tuple_contains<T, std::tuple<>> : std::false_type {};
template<class T, class Head, class... Tail>
struct tuple_contains<T, std::tuple<Head, Tail...>>
  : std::conditional_t<std::is_same_v<T, Head>,
                       std::true_type,
                       tuple_contains<T, std::tuple<Tail...>>> {};
template<class T, class Tuple>
inline constexpr bool tuple_contains_v = tuple_contains<T, Tuple>::value;

// order-insensitive set equality
template<class A, class B, std::size_t... I>
constexpr bool tuple_all_in_impl(std::index_sequence<I...>) {
  return (tuple_contains_v<std::tuple_element_t<I, A>, B> && ...);
}
template<class A, class B>
constexpr bool tuple_set_equal_v =
  (std::tuple_size_v<A> == std::tuple_size_v<B>) &&
  tuple_all_in_impl<A, B>(std::make_index_sequence<std::tuple_size_v<A>>{}) &&
  tuple_all_in_impl<B, A>(std::make_index_sequence<std::tuple_size_v<B>>{});

struct tau : dimension::symbol { static constexpr double value = 6.283185307179586; };

TEST(TempTests, testCombineFactors_diagnostic) {
  using Fsym1 = dimension::factor_t<
    std::ratio<2,1>,
    dimension::symbol_exponent<symbols::pi,  2, 1>,
    dimension::symbol_exponent<tau,         -1, 1>,
    dimension::ratio_exponent<std::ratio<3,5>,  1>
  >;

  using Fsym2 = dimension::factor_t<
    std::ratio<3,1>,
    dimension::symbol_exponent<symbols::pi, -2, 1>,
    dimension::symbol_exponent<tau,          3, 1>,
    dimension::ratio_exponent<std::ratio<3,5>,  2>,
    dimension::ratio_exponent<std::ratio<7,3>, -1>
  >;

  // ---------- MULTIPLY ----------
  using FM          = dimension::multiply_factors_t<Fsym1, Fsym2>;
  using FM_expected = dimension::factor_t<
    std::ratio<6,1>,
    dimension::symbol_exponent<tau, 2, 1>,
    dimension::ratio_exponent<std::ratio<3,5>, 3>,
    dimension::ratio_exponent<std::ratio<7,3>, -1>
  >;

  // Base ratio
  static_assert(std::is_same_v<typename FM::ratio, typename FM_expected::ratio>,
                "FM: base ratio mismatch");

  using FM_sym_actual   = typename FM::symbols;
  using FM_sym_expected = typename FM_expected::symbols;
  using FM_rat_actual   = typename FM::ratios;
  using FM_rat_expected = typename FM_expected::ratios;

  // Order-insensitive (expected to pass)
  static_assert(tuple_set_equal_v<FM_sym_actual, FM_sym_expected>,
                "FM: symbols differ by content");
  static_assert(tuple_set_equal_v<FM_rat_actual, FM_rat_expected>,
                "FM: ratio_exponents differ by content");

  // Optional: order-sensitive (may fail only due to permutation)
  static_assert(tuple_same_v<FM_sym_actual, FM_sym_expected> || true,
                "FM: symbols order differs (content matches)");
  static_assert(tuple_same_v<FM_rat_actual, FM_rat_expected> || true,
                "FM: ratio_exponents order differs (content matches)");


  // ---------- DIVIDE ----------
  using FD          = dimension::divide_factors_t<Fsym1, Fsym2>;
  using FD_expected = dimension::factor_t<
    std::ratio<2,3>,
    dimension::symbol_exponent<symbols::pi, 4, 1>,
    dimension::symbol_exponent<tau, -4, 1>,
    dimension::ratio_exponent<std::ratio<3,5>, -1>,
    dimension::ratio_exponent<std::ratio<7,3>,  1>
  >;

  static_assert(std::is_same_v<typename FD::ratio, typename FD_expected::ratio>,
                "FD: base ratio mismatch");

  using FD_sym_actual   = typename FD::symbols;
  using FD_sym_expected = typename FD_expected::symbols;
  using FD_rat_actual   = typename FD::ratios;
  using FD_rat_expected = typename FD_expected::ratios;

  static_assert(tuple_set_equal_v<FD_sym_actual, FD_sym_expected>,
                "FD: symbols differ by content");
  static_assert(tuple_set_equal_v<FD_rat_actual, FD_rat_expected>,
                "FD: ratio_exponents differ by content");

  static_assert(tuple_same_v<FD_sym_actual, FD_sym_expected> || true,
                "FD: symbols order differs (content matches)");
  static_assert(tuple_same_v<FD_rat_actual, FD_rat_expected> || true,
                "FD: ratio_exponents order differs (content matches)");
}



// Shorthands to pull grouped tuples from factor_t
template<class F> using F_ratio  = typename F::ratio;
template<class F> using F_syms   = typename F::symbols;
template<class F> using F_rates  = typename F::ratios;

// ================================
// Tests
// ================================
TEST(TempTests, combine_identity_and_cancellation_and_associativity) {
  using One = dimension::factor_t<std::ratio<1>>;

  using A = dimension::factor_t<
    std::ratio<2,1>,
    dimension::symbol_exponent<symbols::pi,  2, 1>,
    dimension::ratio_exponent<std::ratio<3,5>,  1>
  >;

  using B = dimension::factor_t<
    std::ratio<3,1>,
    dimension::symbol_exponent<tau, 1, 1>,
    dimension::ratio_exponent<std::ratio<7,3>, -1>
  >;

  using C = dimension::factor_t<
    std::ratio<5,1>,
    dimension::symbol_exponent<symbols::pi, -2, 1>,  // cancels A's pi^2
    dimension::ratio_exponent<std::ratio<3,5>,  2>   // will add with A's to ^3
  >;

  // ---- Identity (One is a left/right identity) ----
  using A1 = dimension::multiply_factors_t<A, One>;
  using A2 = dimension::multiply_factors_t<One, A>;
  static_assert(same_v<F_ratio<A1>, F_ratio<A>> && same_v<F_ratio<A2>, F_ratio<A>>);
  static_assert(tuple_set_equal_v<F_syms<A1>,  F_syms<A>>  && tuple_set_equal_v<F_syms<A2>,  F_syms<A>>);
  static_assert(tuple_set_equal_v<F_rates<A1>, F_rates<A>> && tuple_set_equal_v<F_rates<A2>, F_rates<A>>);

  // ---- Cancellation / zero-drop on symbols and ratio_exponents ----
  using AC   = dimension::multiply_factors_t<A, C>;
  using AC_e = dimension::factor_t<
    std::ratio<10,1>,                                  // 2*5
    //pi cancels out entirely
    dimension::ratio_exponent<std::ratio<3,5>, 3>      // 1 + 2
  >;
  static_assert(same_v<F_ratio<AC>,  F_ratio<AC_e>>);
  static_assert(tuple_set_equal_v<F_syms<AC>,  F_syms<AC_e>>);
  static_assert(tuple_set_equal_v<F_rates<AC>, F_rates<AC_e>>);

  // ---- Associativity (content-wise) ----
  using Left  = dimension::multiply_factors_t<dimension::multiply_factors_t<A,B>, C>;
  using Right = dimension::multiply_factors_t<A, dimension::multiply_factors_t<B,C>>;
  static_assert(same_v<F_ratio<Left>, F_ratio<Right>>, "Associativity: base ratios mismatch");
  static_assert(tuple_set_equal_v<F_syms<Left>,  F_syms<Right>>,  "Associativity: symbols differ");
  static_assert(tuple_set_equal_v<F_rates<Left>, F_rates<Right>>, "Associativity: ratio_exponents differ");
}

TEST(TempTests, combine_divide_is_inverse_of_multiply) {
  using A = dimension::factor_t<
    std::ratio<6,1>,
    dimension::symbol_exponent<symbols::pi,  3, 1>,
    dimension::symbol_exponent<tau,         -2, 1>,
    dimension::ratio_exponent<std::ratio<3,5>,  1>,
    dimension::ratio_exponent<std::ratio<7,3>, -1>
  >;

  using B = dimension::factor_t<
    std::ratio<2,1>,
    dimension::symbol_exponent<symbols::pi, -1, 1>,
    dimension::symbol_exponent<tau,          5, 1>,
    dimension::ratio_exponent<std::ratio<3,5>,  2>
  >;

  using AB   = dimension::multiply_factors_t<A,B>;
  using AB_d = dimension::divide_factors_t<AB, B>; // should return A (modulo ordering)

  static_assert(same_v<F_ratio<AB_d>, F_ratio<A>>, "Divide inverse: base ratio mismatch");
  static_assert(tuple_set_equal_v<F_syms<AB_d>,  F_syms<A>>,  "Divide inverse: symbols differ");
  static_assert(tuple_set_equal_v<F_rates<AB_d>, F_rates<A>>, "Divide inverse: ratio_exponents differ");
}

TEST(TempTests, reduce_factor_basic_collapse) {
  using F = dimension::factor_t<
    std::ratio<1,1>,
    // symbols: pi^2 * pi^-1 * tau^1 * tau^1  -> pi^1, tau^2
    dimension::symbol_exponent<symbols::pi,  2, 1>,
    dimension::symbol_exponent<symbols::pi, -1, 1>,
    dimension::symbol_exponent<tau,          1, 1>,
    dimension::symbol_exponent<tau,          1, 1>,
    // ratios: (3/5)^1 * (3/5)^2 * (7/3)^-1 * (7/3)^1 -> (3/5)^3, (7/3)^0 drops
    dimension::ratio_exponent<std::ratio<3,5>,  1>,
    dimension::ratio_exponent<std::ratio<3,5>,  2>,
    dimension::ratio_exponent<std::ratio<7,3>, -1>,
    dimension::ratio_exponent<std::ratio<7,3>,  1>
  >;

  using R = typename dimension::reduce_factor<F>::type;

  using R_expected = dimension::factor_t<
    std::ratio<1,1>,
    dimension::symbol_exponent<symbols::pi, 1, 1>,
    dimension::symbol_exponent<tau,         2, 1>,
    dimension::ratio_exponent<std::ratio<3,5>, 3>
  >;

  static_assert(same_v<typename R::ratio, typename R_expected::ratio>, "base ratio changed unexpectedly");
  static_assert(tuple_set_equal_v<typename R::symbols, typename R_expected::symbols>, "symbols collapse mismatch");
  static_assert(tuple_set_equal_v<typename R::ratios,  typename R_expected::ratios>,  "ratio_exponents collapse mismatch");
}

TEST(TempTests, reduce_factor_zero_drop_and_cvref_noise) {
  using F = dimension::factor_t<
    std::ratio<5,2>,
    const dimension::symbol_exponent<symbols::pi,  1, 1>&,
    volatile dimension::symbol_exponent<symbols::pi, -1, 1>,
    dimension::ratio_exponent<std::ratio<9,4>, 0>
  >;

  using R = typename dimension::reduce_factor<F>::type;

  using R_expected = dimension::factor_t<
    std::ratio<5,2>
    // pi^(1-1) drops, ratio_exponent<9/4,0> drops → empty exponent tuples
  >;

  static_assert(same_v<typename R::ratio, typename R_expected::ratio>);
  static_assert(std::tuple_size_v<typename R::symbols> == 0, "zero symbol exponent not dropped");
  static_assert(std::tuple_size_v<typename R::ratios>  == 0, "zero ratio exponent not dropped");
}

TEST(TempTests, reduce_factor_idempotent) {
  using F = dimension::factor_t<
    std::ratio<3,1>,
    dimension::symbol_exponent<tau, 2, 1>,
    dimension::ratio_exponent<std::ratio<5,7>, -3>
  >;

  using R1 = typename dimension::reduce_factor<F>::type;
  using R2 = typename dimension::reduce_factor<R1>::type;

  static_assert(same_v<typename R1::ratio, typename R2::ratio>);
  static_assert(tuple_set_equal_v<typename R1::symbols, typename R2::symbols>);
  static_assert(tuple_set_equal_v<typename R1::ratios,  typename R2::ratios>);
}

TEST(TempTests, reduce_factor_normalizes_ratio_bases) {
  using F = dimension::factor_t<
    std::ratio<1,1>,
    dimension::ratio_exponent<std::ratio<7,3>,   1>,
    dimension::ratio_exponent<std::ratio<14,6>, -1>
  >;

  using R = typename dimension::reduce_factor<F>::type;

  // Expect both to cancel → no ratio_exponents remain
  static_assert(std::tuple_size_v<typename R::ratios> == 0, "ratio_exponents not collapsed after base normalization");
  // symbols untouched
  static_assert(std::tuple_size_v<typename R::symbols> == 0, "symbols unexpectedly present");
  // base ratio unchanged
  static_assert(std::is_same_v<typename R::ratio, std::ratio<1,1>>);
}

TEST(TempTests, combine_normalizes_ratio_bases) {
  using A = dimension::factor_t<
    std::ratio<1,1>,
    dimension::ratio_exponent<std::ratio<7,3>,   1>
  >;
  using B = dimension::factor_t<
    std::ratio<1,1>,
    dimension::ratio_exponent<std::ratio<14,6>, -1>
  >;

  using M = dimension::multiply_factors_t<A,B>;
  static_assert(std::tuple_size_v<typename M::ratios> == 0);

  using D = dimension::divide_factors_t<A,A>;
  static_assert(std::tuple_size_v<typename D::ratios> == 0);
}


TEST(TempTests, testApplyAllConversions){

   // This should be a set of conversions between two sets of units.
   // We need to consider the following cases:
   //   One unit
   //   Multi unit
   //   Different order
   //   Derived units
   //   Labelled units (interpreted here as powers/compound forms; skip prefixes unless they exist in your unit set)
   //   Identity / no-op
   //   Empty sets
   //   Cancellation across numerator/denominator

   // --- One unit ---
   {
      using conv = apply_all_conversions<std::tuple<unit_exponent<meters>>, std::tuple<unit_exponent<feet>>>;
      static_assert(std::is_same_v<typename conv::factor::ratio, std::ratio<1250, 381>>);
   }

   // --- Multi unit (product): (m · s) -> (ft · s) === 1250/381 ---
   {
      using conv = apply_all_conversions<
         std::tuple<unit_exponent<meters>, unit_exponent<seconds>>,
         std::tuple<unit_exponent<feet>,   unit_exponent<seconds>>
      >;
      static_assert(std::is_same_v<typename conv::factor::ratio, std::ratio<1250, 381>>);
   }

   // --- Different order: (s · m) -> (ft · s) === 1250/381 ---
   {
      using conv = apply_all_conversions<
         std::tuple<unit_exponent<seconds>, unit_exponent<meters>>,
         std::tuple<unit_exponent<feet>,    unit_exponent<seconds>>
      >;
      static_assert(std::is_same_v<typename conv::factor::ratio, std::ratio<1250, 381>>);
   }

   // --- Derived-like (velocity): (m / s) -> (ft / s) === 1250/381 ---
   {
      using conv = apply_all_conversions<
         std::tuple<unit_exponent<meters>, unit_exponent<seconds, -1>>,
         std::tuple<unit_exponent<feet>,   unit_exponent<seconds, -1>>
      >;
      static_assert(std::is_same_v<typename conv::factor::ratio, std::ratio<1250, 381>>);
   }

   // --- Derived-like (area): m^2 -> ft^2 === (1250/381)^2 ---
   {
      using conv = apply_all_conversions<
         std::tuple<unit_exponent<meters, 2>>,
         std::tuple<unit_exponent<feet,   2>>
      >;

      // Expected via exact integers
      using expected_ratio = std::ratio<1562500, 145161>;

      // Or computed via your raise_ratio_t for clarity
      using feet_per_meter = std::ratio<1250, 381>;
      using expected_via_raise = dimension::raise_ratio_t<feet_per_meter, 2>;

      // Prefer value equality to tolerate representation differences
      static_assert(std::ratio_equal_v<typename conv::factor::ratio, expected_ratio>,
                  "m^2 -> ft^2: base ratio mismatch");

      static_assert(std::ratio_equal_v<typename conv::factor::ratio, expected_via_raise>,
                  "m^2 -> ft^2: mismatch vs raise_ratio_t(feet_per_meter, 2)");
   }

   // --- Cancellation across numerator/denominator: (m · s^-1) -> (ft · s^-1) === 1250/381 ---
   {
      using conv = apply_all_conversions<
         std::tuple<unit_exponent<meters>, unit_exponent<seconds, -1>>,
         std::tuple<unit_exponent<feet>,   unit_exponent<seconds, -1>>
      >;
      static_assert(std::is_same_v<typename conv::factor::ratio, std::ratio<1250, 381>>);
   }

   // --- Identity / no-op: same sets => 1 ---
   {
      using conv = apply_all_conversions<
         std::tuple<unit_exponent<meters>, unit_exponent<seconds, -2>>,
         std::tuple<unit_exponent<meters>, unit_exponent<seconds, -2>>
      >;
      static_assert(std::is_same_v<typename conv::factor::ratio, std::ratio<1, 1>>);
   }

   // --- Empty sets => 1 (dimensionless to dimensionless) ---
   {
      using conv = apply_all_conversions<std::tuple<>, std::tuple<>>;
      static_assert(std::is_same_v<typename conv::factor::ratio, std::ratio<1, 1>>);
   }

   // --- Mixed compound with reordering on both sides (valid):
   // LHS:  (s · m · s^-2)    -> simplifies to (m · s^-1)
   // RHS:  (s^-3 · ft · s^2) -> simplifies to (ft · s^-1)
   // Expected factor: meters -> feet = 1250/381
   {
      using conv = apply_all_conversions<
         std::tuple<
            unit_exponent<seconds>,
            unit_exponent<meters>,
            unit_exponent<seconds, -2>
         >,
         std::tuple<
            unit_exponent<seconds, -3>,
            unit_exponent<feet>,
            unit_exponent<seconds, 2>
         >
      >;
      static_assert(std::is_same_v<typename conv::factor::ratio, std::ratio<1250, 381>>);
   }
}



TEST(TempTests, testNamedGetter){

   // The problem seems to specifically be related to the custom "fundamental" units...
   //   After adding acres, this very much seems to be the case.
   //   After further validation, I think I found the culprit.
   //   These "special" units have conversion factors with many, many digits.
   //   I think this is causing an overflow.
   //   Its unfortunate this is even possible...
   //   For now, I may just have to reduce the precision until I have a better solution.

   //volume<meters> obj(1.0);
   //ASSERT_NEAR((get_volume_as<cups>(obj)), 128.0, 1e-3);

   // WORKS
   //speed<knots> obj(1.0);
   //ASSERT_NEAR((get_speed_as<knots>(obj)), 128.0, 1e-3);

   //frequency<hertz> obj(1.0);
   //ASSERT_NEAR((get_frequency_as<hertz>(obj)), 128.0, 1e-3);

   area<meters> obj(1.0);
   ASSERT_NEAR((get_area_as<acres>(obj)), 128.0, 1e-3);
}
*/



TEST(TempTests, testRatioExponentConversions){
    constexpr base_dimension<unit_exponent<meters>> obj1(10.0);
    constexpr base_dimension<unit_exponent<feet>> obj2(5.0);

    std::cout << obj1 << std::endl;
    std::cout << obj2 << std::endl;

    // Wrong...
    //static_assert(obj1 < obj2, "Fail");

    using feet_to_meters_factor = typename details::factor::get_factor_type<feet, meters>::type;

    std::cout << "Conversion factor from meters to feet: " << factor::eval_factor<typename Conversion<meters, feet>::scale>() << std::endl;


    std::cout << "Conversion factor from feet to meters: " << factor::eval_factor<feet_to_meters_factor>() << std::endl; // This is printing the wrong value, its off by a factor of 10, with is the ratio_exponent
    /*
    static_assert(std::is_same_v<typename feet_to_meters_factor::ratio, std::ratio<381, 125>>);
    static_assert(std::tuple_size_v<typename feet_to_meters_factor::symbols> == 0);
    //static_assert(std::tuple_size_v<typename feet_to_meters_factor::ratios> == 1); // Fails - the ratio_exponent isn't propagated.
    
    static_assert(std::tuple_size_v<typename Conversion<meters, feet>::scale::ratios> == 1); // Works, so the ratio_exponent definitely exists in the original conversion, but is lost when reversing.
    using inverted_ratio_exopnents = tuple_invert_ratio_exponents_t<typename Conversion<meters, feet>::scale::ratios>;
    static_assert(std::tuple_size_v<inverted_ratio_exopnents> == 1); // Works, so inverting it is fine...
    */
    // Ok, the problem seems to be that ratio_exponents are being provided as a tuple, but the signature expects a parameter pack... maybe?

    std::cout << "Meters object in meters: " << get_dimension_as<unit_exponent<meters>>(obj1) << std::endl;
    std::cout << "Meters object in feet: " << get_dimension_as<unit_exponent<feet>>(obj1) << std::endl;
    std::cout << "Feet object in feet: " << get_dimension_as<unit_exponent<feet>>(obj2) << std::endl;
    std::cout << "Feet object in meters: " << get_dimension_as<unit_exponent<meters>>(obj2) << std::endl; // This one is failing... showing 15.24 rather than 1.524.. so the ratio_exponent isn't applying in this case?

}



























// Helper to print a single ratio_exponent
template<typename RE>
void print_ratio_exponent()
{
    std::cout
        << "ratio: " << RE::ratio::num << "/" << RE::ratio::den
        << ", exponent: " << RE::exponent::num << "/" << RE::exponent::den << "\n";
}

// Print all ratio_exponents in a tuple
template<typename... REs>
void print_tuple(const std::tuple<REs...>&)
{
    (print_ratio_exponent<REs>(), ...); // Fold expression
}





TEST(FundamentalConversions, TestTempTimeConversions)
{
   // Test from seconds to other units
   timespan<seconds> timeInseconds{3600.0};  // 1 hour in seconds

   EXPECT_NEAR(get_timespan_as<seconds>(timeInseconds), 3600.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<minutes>(timeInseconds), 60.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<hours>(timeInseconds), 1.0, TOLERANCE);

   // Test from minutes to other units
   timespan<minutes> timeInminutes{120.0};  // 2 hours in minutes

//   EXPECT_NEAR(get_timespan_as<seconds>(timeInminutes), 7200.0, TOLERANCE);
   EXPECT_NEAR(get_dimension_as<unit_exponent<seconds>>(timeInminutes), 7200.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<minutes>(timeInminutes), 120.0, TOLERANCE);
   EXPECT_NEAR(get_timespan_as<hours>(timeInminutes), 2.0, TOLERANCE);


   
   using simplified_units = simplified_units_t<typename decltype(timeInminutes)::units>;
   using conv_factor = typename apply_all_conversions<simplified_units, std::tuple<unit_exponent<seconds>>>::factor;
   //constexpr auto conversion = factor::eval_factor<conv_factor, double>();

   static_assert(std::is_same_v<simplified_units, std::tuple<unit_exponent<minutes>>>);

   static_assert(is_tuple_v<typename conv_factor::ratios>);

   std::cout << "second to minutes: " << std::endl;
   print_tuple(Conversion<seconds, minutes>::scale::ratios{});

   std::cout << "full conversion: " << std::endl;
   print_tuple(conv_factor::ratios{});

   std::cout << "radians to degrees: " << std::endl;
   print_tuple(Conversion<radians, degrees>::scale::ratios{});

   //static_assert(is_tuple_v<typename conv_factor::ratios>);
   

   //using f = typename get_dimension_as<unit_exponent<seconds>>::factor;

}