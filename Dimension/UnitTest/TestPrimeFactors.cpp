// ===== Dimension/UnitTest/TestPrimeFactors.cpp =====
#include "DimensionTest.h"

using namespace dimension;

// Handy alias to read the ratio_exponents tuple off any dimensional expression type.
template <class Q>
using ratio_tuple_t = typename Q::ratio_exponents;

// Minimal helpers to assert tuple size and equality of the ratio_exponents sets.
template <class T>
constexpr std::size_t ratio_tuple_size_v = std::tuple_size_v<ratio_tuple_t<T>>;

template <class A, class B>
constexpr bool same_ratio_tuple_v = std::is_same_v<ratio_tuple_t<A>, ratio_tuple_t<B>>;

TEST(PrimeFactors, InchOverMeter_NoSimplifyWithAuto)
{
    auto q = length<inches>(1.0) / length<meters>(1.0);
    // Stays as inches/meters (not dimensionless), so no prime factor tuple yet:
    static_assert(!std::is_same_v<typename decltype(q)::units, std::tuple<>>, "Units should not cancel with auto");
    static_assert(ratio_tuple_size_v<decltype(q)> == 0, "No conversion invoked yet; ratio_exponents should be empty");
    SUCCEED();
}

TEST(PrimeFactors, FootOverMeter_NoSimplifyWithAuto)
{
    auto q = length<feet>(1.0) / length<meters>(1.0);
    static_assert(!std::is_same_v<typename decltype(q)::units, std::tuple<>>, "Units should not cancel with auto");
    static_assert(ratio_tuple_size_v<decltype(q)> == 0, "No conversion invoked yet; ratio_exponents should be empty");
    SUCCEED();
}

// --- Force simplify by assigning to timespan ---
TEST(PrimeFactors, InchOverMeter_ForcesPrimeTupleWhenCancelling)
{
    timespan t = length<inches>(1.0) / speed<meters, seconds>(1.0);
    // Expect {2, 5, 127}
    static_assert(ratio_tuple_size_v<decltype(t)> == 3, "Expected three prime bases in inch↔meter after cancellation");
    SUCCEED();
}

TEST(PrimeFactors, FootOverMeter_ForcesPrimeTupleWhenCancelling)
{
    constexpr auto t_orig = length<feet>(1.0) / speed<meters, seconds>(1.0);
    constexpr timespan<seconds> t_canon = t_orig;
    static_assert(t_canon.get_raw() == 0.3048);
    static_assert(std::tuple_size_v<typename decltype(t_canon)::ratio_exponents> == 0);
    using dimType = simplify_dimension_t<decltype(t_orig)>;
    static_assert(std::is_same_v<dimType::units, std::tuple<unit_exponent<seconds>>>, "Units should be seconds");
    static_assert(std::tuple_size_v<dimType::ratio_exponents> == 4, "Expected four prime bases in foot↔meter after cancellation");
    constexpr timespan t = t_orig;
    static_assert(get_timespan_as<seconds>(t) == 0.3048);
    static_assert(t.get_raw() == 1.0);
    // Expect {2, 3, 5, 127}
    static_assert(std::is_same_v<typename decltype(t)::units, std::tuple<unit_exponent<seconds>>>, "Units should be seconds");
    static_assert(std::tuple_size_v<typename decltype(t)::ratio_exponents> == 4, "Expected four prime bases in foot↔meter after cancellation");
    SUCCEED();
}

TEST(PrimeFactors, VerifySimplification)
{
    constexpr auto t_orig = length<feet>(1.0) / speed<meters, seconds>(1.0);
    using dimType = simplify_dimension_t<decltype(t_orig)>;
    static_assert(std::is_same_v<dimType::units, std::tuple<unit_exponent<seconds>>>, "Units should be seconds");
    static_assert(std::tuple_size_v<dimType::ratio_exponents> == 4, "Expected four prime bases in foot↔meter after cancellation");
    SUCCEED();
}

// --- Composition path independence: different algebraic routes yield the same reduced ratio_exponents type ---
TEST(PrimeFactors, Normalization_PathIndependence)
{
    // Direct: inches over meters
    auto direct = length<inches>(1.0) / length<meters>(1.0);

    // Via feet: (ft/m) / (ft/in)  should equal (in/m) after full reduction
    auto via_feet = (length<feet>(1.0) / length<meters>(1.0)) /
                    (length<feet>(1.0) / length<inches>(1.0));

    static_assert(same_ratio_tuple_v<decltype(direct), decltype(via_feet)>,
                  "Prime-factor tuple must normalize identically across algebraic paths");

    // Runtime double-check the numeric value too (should be 0.0254)
    EXPECT_NEAR(static_cast<double>(direct), static_cast<double>(via_feet), 0.0);
}

// --- Full cancellation: compose a factor with its exact inverse, the prime set must collapse to empty ---
TEST(PrimeFactors, Cancellation_ToIdentity)
{
    auto a = length<inches>(1.0) / length<meters>(1.0);
    auto b = length<meters>(1.0) / length<inches>(1.0);
    auto id = a * b;

    // All prime bases should cancel out
    static_assert(ratio_tuple_size_v<decltype(id)> == 0, "Expected empty prime set after cancellation");

    // And at runtime it must be exactly 1.0 (dimensionless)
    EXPECT_DOUBLE_EQ(static_cast<double>(id), 1.0);
}

// --- Associativity/commutativity with mixed units should not change the reduced prime set ---
TEST(PrimeFactors, MixedComposition_NormalizesIdentically)
{
    // Two algebraically equivalent expressions formed from inches, feet, and meters:
    // E1 = (in/m) * (m/ft)
    auto e1 = (length<inches>(1.0) / length<meters>(1.0)) *
              (length<meters>(1.0) / length<feet>(1.0));

    // E2 = in/ft
    auto e2 = length<inches>(1.0) / length<feet>(1.0);

    // After reduction, both represent 1/12 with the same prime support {2,3}
    static_assert(same_ratio_tuple_v<decltype(e1), decltype(e2)>,
                  "Equivalent expressions must yield identical reduced prime tuples");

    EXPECT_NEAR(static_cast<double>(e1), static_cast<double>(e2), 0.0);
}

// --- Exponentiation should scale exponents but keep the prime support set identical ---
TEST(PrimeFactors, Powers_PreservePrimeSupportSet)
{
    // a = in/m  (three primes) ; a^2 should have the same set of primes, just doubled exponents
    auto a  = length<inches>(1.0) / length<meters>(1.0);
    auto a2 = a * a;

    static_assert(ratio_tuple_size_v<decltype(a)>  == ratio_tuple_size_v<decltype(a2)>,
                  "Prime support set cardinality should be unchanged under squaring");

    // Numeric check: (in/m)^2 equals (in^2 / m^2)
    auto area_in2_over_m2 = (length<inches>(1.0) * length<inches>(1.0)) /
                            (length<meters>(1.0)  * length<meters>(1.0));
    EXPECT_NEAR(static_cast<double>(a2), static_cast<double>(area_in2_over_m2), 0.0);
}
