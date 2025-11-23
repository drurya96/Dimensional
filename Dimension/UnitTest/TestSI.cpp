// ===== Dimension/UnitTest/TestSI.cpp =====
#include "DimensionTest.h"

using namespace dimension;

TEST(SI_Prefix_Length, ToSmallerPrefixes_FromMeters)
{
    length<meters> one_m{1.0};

    EXPECT_NEAR(get_length_as<milli_meters>(one_m), 1'000.0, TOLERANCE);
    EXPECT_NEAR(get_length_as<centi_meters>(one_m),   100.0, TOLERANCE);
    EXPECT_NEAR(get_length_as<deci_meters>(one_m),     10.0, TOLERANCE);
    EXPECT_NEAR(get_length_as<micro_meters>(one_m), 1e6,     TOLERANCE);
    EXPECT_NEAR(get_length_as<nano_meters>(one_m),  1e9,     TOLERANCE);
    EXPECT_NEAR(get_length_as<pico_meters>(one_m),  1e12,    TOLERANCE);
}

TEST(SI_Prefix_Length, ToLargerPrefixes_FromMeters)
{
    length<meters> one_m{1.0};

    EXPECT_NEAR(get_length_as<deca_meters>(one_m),  0.1,      TOLERANCE);
    EXPECT_NEAR(get_length_as<hecto_meters>(one_m), 0.01,     TOLERANCE);
    EXPECT_NEAR(get_length_as<kilo_meters>(one_m),  0.001,    TOLERANCE);
    EXPECT_NEAR(get_length_as<mega_meters>(one_m),  1e-6,     TOLERANCE);
    EXPECT_NEAR(get_length_as<giga_meters>(one_m),  1e-9,     TOLERANCE);
    EXPECT_NEAR(get_length_as<tera_meters>(one_m),  1e-12,    TOLERANCE);
}

TEST(SI_Prefix_Length, FromPrefixedToMeters)
{
    EXPECT_NEAR(get_length_as<meters>(length<milli_meters>{1234.0}), 1.234,       TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<centi_meters>{250.0}),  2.5,         TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<deci_meters>{37.0}),    3.7,         TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<deca_meters>{3.5}),     35.0,        TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<hecto_meters>{1.2}),    120.0,       TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<kilo_meters>{0.015}),   15.0,        TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<micro_meters>{5.0e6}),  5.0,         TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<nano_meters>{3.2e9}),   3.2,         TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<pico_meters>{7.0e12}),  7.0,         TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<mega_meters>{2.0e-3}),  2000.0,      TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<giga_meters>{4.0e-6}),  4000.0,      TOLERANCE);
    EXPECT_NEAR(get_length_as<meters>(length<tera_meters>{9.0e-9}),  9000.0,      TOLERANCE);
}

TEST(SI_Prefix_Length, CrossPrefixRoundTrip)
{
    // Start in pico, convert to tera, back to meters—path independence on pure powers of 10
    length<pico_meters> lp{3.25e11}; // = 0.325 meters
    length<tera_meters> lt{lp};
    length<meters> back{lt};

    EXPECT_NEAR(get_length_as<meters>(back), 0.325, TOLERANCE);

    // Arbitrary value across micro -> kilo -> nano -> meters
    length<micro_meters> a{7.5e6};   // 7.5 meters
    length<kilo_meters>  b{a};       // 0.0075 km
    length<nano_meters>  c{b};       // 7.5e9 nm
    length<meters>       d{c};       // 7.5 m
    EXPECT_NEAR(get_length_as<meters>(d), 7.5, TOLERANCE);
}

TEST(SI_Prefix_Length, NegativeAndZero)
{
    length<kilo_meters> km_zero{0.0};
    EXPECT_NEAR(get_length_as<meters>(km_zero), 0.0, TOLERANCE);

    length<centi_meters> cm_neg{-12.5}; // -0.125 m
    EXPECT_NEAR(get_length_as<meters>(cm_neg), -0.125, TOLERANCE);
    EXPECT_NEAR(get_length_as<milli_meters>(cm_neg), -125.0, TOLERANCE);
}

TEST(SI_Prefix_Length, MixedPrefixArithmetic_ProducesCorrectBase)
{
    // 2 km * 3 dm = (2000 m) * (0.3 m) = 600 m^2
    length<kilo_meters> a{2.0};
    length<deci_meters> b{3.0};
    area<meters> A = a * b;
    EXPECT_NEAR(get_area_as<meters>(A), 600.0, TOLERANCE);

    // (500 mm) / (2 km) = 0.5 m / 2000 m = 0.00025 (dimensionless)
    base_dimension<> r = length<milli_meters>(500.0) / length<kilo_meters>(2.0);
    EXPECT_NEAR(r, 2.5e-4, TOLERANCE);
}

TEST(SI_Prefix_Length, CompileTime_ConstexprSanity)
{
    constexpr length<kilo_meters> akm{1.0};      // 1000 m
    constexpr length<milli_meters> bmm{250'000}; // 250 m
    constexpr length<meters> sum = length<meters>(akm) + length<meters>(bmm);
    static_assert(get_length_as<meters>(sum) == 1250.0, "constexpr prefix sum failed");

    // Identity through two opposite prefixes: m -> km -> m
    constexpr length<meters> m1{42.0};
    constexpr length<kilo_meters> km1{m1};
    constexpr length<meters> m2{km1};
    static_assert(get_length_as<meters>(m2) == 42.0, "constexpr round-trip failed");
}
