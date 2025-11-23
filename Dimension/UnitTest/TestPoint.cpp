#include "DimensionTest.h"

#include <iostream>
/*
// point_tests.cpp
#include <gtest/gtest.h>

#include "point.h"
#include "point_ops.h"
#include "point_convert_impl.h"
#include "frame_traits.h"
#include "temperatureExtras.h"   // defines celsius/fahrenheit frames & K/R offsets
*/
// If your units live in namespace dimension::
using namespace dimension;

// ------------------------- tiny helpers -------------------------
constexpr double EPS = 1e-9;

template <class Target, class Frame, class Dim>
static void EXPECT_POINT_NEAR(const point<Frame, Dim>& p, double expected) {
    EXPECT_NEAR(get_point_as<Target>(p), expected, EPS);
}

template <class Unit, class Q>
static void EXPECT_Q_NEAR(const Q& q, double expected) {
    // For temperature<>, use get_temperature_as; for general, use get_dimension_as
    // Prefer the general path by default:
    EXPECT_NEAR((get_dimension_as<unit_exponent<Unit>>(q)), expected, EPS);
}

// SFINAE/constraints checks without hard failing the build
template <class L, class R>
concept has_plus = requires(L a, R b) { a + b; };

template <class L, class R>
concept has_get_point_as = requires(L a) { get_point_as<R>(a); };

// ------------------------- basic construction/identity -------------------------
TEST(PointBasics, ConstructionAndIdentity) {
    point<celsius, temperatureType> c{10.0};
    EXPECT_POINT_NEAR<celsius>(c, 10.0);

    point<kelvin, temperatureType> k{283.15};
    EXPECT_POINT_NEAR<kelvin>(k, 283.15);

    // converting ctor: K -> C
    point<celsius, temperatureType> c_from_k{k};
    EXPECT_POINT_NEAR<celsius>(c_from_k, 10.0);

    // converting ctor: C -> K
    point<kelvin, temperatureType> k_from_c{c};
    EXPECT_POINT_NEAR<kelvin>(k_from_c, 283.15);
}

// ------------------------- six-case conversion matrix -------------------------
TEST(PointConversions, SixCaseMatrix) {
    // Use a value that exercises offsets well.
    point<celsius, temperatureType> pc{10.0};

    // 1) same frame → same frame
    EXPECT_POINT_NEAR<celsius>(pc, 10.0);

    // 2) frame → its unit (C→K) : +273.15
    EXPECT_POINT_NEAR<kelvin>(pc, 283.15);

    // 3) frame → other unit (C→R) : (C+273.15)*9/5
    EXPECT_POINT_NEAR<rankine>(pc, (10.0 + 273.15) * 9.0/5.0);

    // 4) frame A → frame B (C→F) : (C+273.15)*9/5 − 459.67
    EXPECT_POINT_NEAR<fahrenheit>(pc, (10.0 + 273.15) * 9.0/5.0 - 459.67);

    // 5) unit (same as target frame’s unit) → target frame (K→C) : −273.15
    point<kelvin, temperatureType> pk{283.15};
    EXPECT_POINT_NEAR<celsius>(pk, 10.0);

    // 6) unit (different) → target frame (R→C) : R*5/9 − 273.15
    // Pick 509.67 R → 50 °F ↔ 10 °C
    point<rankine, temperatureType> pr{(50.0 + 459.67)};
    EXPECT_POINT_NEAR<celsius>(pr, 10.0);
}

// ------------------------- arithmetic semantics -------------------------
TEST(PointArithmetic, TranslateAndDifference) {
    point<kelvin,    temperatureType> tK{298.15}; // 25 C
    point<celsius,   temperatureType> tC{10.0};
    temperature<kelvin> dT{10.0};                 // 10 K

    // point ± quantity → point  (result stays in the point's frame)
    auto p1 = tK + dT;  // Kelvin frame
    EXPECT_POINT_NEAR<celsius>(p1, 35.0);         // 298.15K +10K = 308.15K = 35 C

    auto p2 = dT + tK;
    EXPECT_POINT_NEAR<celsius>(p2, 35.0);

    auto p3 = tK - dT;
    EXPECT_POINT_NEAR<celsius>(p3, 15.0);

    // point − point → quantity in LHS frame’s unit
    auto qK = tK - tC; // should be 15 K
    EXPECT_NEAR(get_temperature_as<kelvin>(qK), 15.0, EPS);

    // Mixed-dimension ops (use existing patterns from your placeholder tests)
    auto a  = timespan<seconds>(2.0) / dT;   // s·K^-1
    EXPECT_NEAR((get_dimension_as<unit_exponent<seconds>, unit_exponent<kelvin, -1>>(a)), 0.2, EPS);

    auto a1 = tK * a;  // seconds
    EXPECT_NEAR(get_timespan_as<seconds>(a1), 59.63, EPS);

    auto a2 = a * tK;  // seconds
    EXPECT_NEAR(get_timespan_as<seconds>(a2), 59.63, EPS);

    auto b  = timespan<seconds>(2.0) * dT;  // s·K
    EXPECT_NEAR((get_dimension_as<unit_exponent<seconds>, unit_exponent<kelvin>>(b)), 20.0, EPS);

    auto b1 = b / tK; // seconds
    EXPECT_NEAR(get_timespan_as<seconds>(b1), 0.067080328693610599, EPS);

    auto c  = dT / timespan<seconds>(2.0); // K·s^-1
    EXPECT_NEAR((get_dimension_as<unit_exponent<kelvin>, unit_exponent<seconds, -1>>(c)), 5.0, EPS);

    auto c1 = tK / c; // seconds
    EXPECT_NEAR(get_timespan_as<seconds>(c1), 59.63, EPS);
}

// ------------------------- round-trip and idempotence -------------------------
TEST(PointRoundTrip, RoundTrips) {
    point<celsius, temperatureType> pc{37.0};

    // C → F → C
    point<fahrenheit, temperatureType> pf{pc};
    point<celsius,    temperatureType> backC{pf};
    EXPECT_POINT_NEAR<celsius>(backC, 37.0);

    // C → K → C
    point<kelvin, temperatureType> pk{pc};
    point<celsius, temperatureType> backC2{pk};
    EXPECT_POINT_NEAR<celsius>(backC2, 37.0);

    // Add–subtract neutrality
    temperature<kelvin> delta{12.5};
    auto moved = pc + delta;
    auto back  = moved - delta;
    EXPECT_POINT_NEAR<celsius>(back, 37.0);
}

// ------------------------- edge numerics -------------------------
TEST(PointEdge, NegativeZeroLarge) {
    // −40 C == −40 F
    point<celsius, temperatureType> m40C{-40.0};
    EXPECT_POINT_NEAR<fahrenheit>(m40C, -40.0);

    // Zero
    point<celsius, temperatureType> zC{0.0};
    EXPECT_POINT_NEAR<kelvin>(zC, 273.15);

    // Large magnitude stability: two paths should agree closely
    point<celsius, temperatureType> bigC{1'000'000.0};
    double directF = get_point_as<fahrenheit>(bigC);
    // via K then to F
    point<kelvin, temperatureType> viaK{bigC};
    double viaKF = get_point_as<fahrenheit>(viaK);
    EXPECT_NEAR(directF, viaKF, 1e-6); // slightly looser epsilon for huge values
}

// ------------------------- units as zero-offset frames -------------------------
TEST(PointUnitsAsFrames, LengthSanity) {
    // Treat pure units as frames; offsets = 0
    point<meters, lengthType> pm{3.0};
    // 3 meters in feet ≈ 9.842519685...
    EXPECT_NEAR(get_point_as<feet>(pm), 9.84251968503937, 1e-10);

    // Difference of points → length quantity in LHS frame's unit
    point<feet, lengthType> pf{get_point_as<feet>(pm)}; // same location expressed in ft
    auto diff = pf - pm; // length<feet>
    EXPECT_Q_NEAR<feet>(diff, 0.0); // exactly same location → zero vector

    // Baseline against origin: (pm − 0_m) == 3 m
    point<meters, lengthType> origin_m{0.0};
    auto d_m = pm - origin_m;
    EXPECT_Q_NEAR<meters>(d_m, 3.0);
}

// ------------------------- frame trait wiring -------------------------
struct origin_K {
    using unit = kelvin;
    static constexpr double offset = 0.0;
};

TEST(PointTraits, OffsetsAndUnits) {
    static_assert(std::is_same_v<frame_unit_t<celsius>,  kelvin>);
    static_assert(std::is_same_v<frame_unit_t<fahrenheit>, rankine>);
    static_assert(std::is_same_v<frame_unit_t<origin_K>, kelvin>);

    EXPECT_DOUBLE_EQ(point_offset_v<celsius>,    273.15);
    EXPECT_DOUBLE_EQ(point_offset_v<fahrenheit>, 459.67);
    EXPECT_DOUBLE_EQ(point_offset_v<origin_K>,     0.0);
}
