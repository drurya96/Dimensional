#include "DimensionTest.h"

using namespace dimension;
using namespace std;

TEST(SymbolicConstruction, ValueAndPi) {
    auto l = make_length<meters>(2.0, symbols::pi{});
    static_assert(std::is_same_v<
        typename decltype(l)::symbols,
        std::tuple<symbol_exponent<symbols::pi,1,1>>>);

    EXPECT_NEAR(get_length_as<meters>(l), 2.0 * 3.1415, 1e-12);
}

TEST(Arithmetic, MultiplyAndCancel) {
    auto a = make_length<meters>(symbols::pi{});
    auto b = make_length<meters>(symbols::pi{});
    auto prod = a * b;                     // π² m²

    auto ratio = prod / prod;              // dimensionless 1
    EXPECT_DOUBLE_EQ(get_dimension_as<>(ratio), 1.0);
    static_assert(std::tuple_size_v<typename decltype(ratio)::symbols> == 0);
}

TEST(CTAD, CoefficientsPreservedThroughAuto) {
    auto a = make_length<meters>(10.0, symbols::pi{});
    auto b = a * a;
    area c = b;
    static_assert(std::tuple_size_v<typename decltype(b)::symbols> == 1);

    EXPECT_NEAR(get_area_as<meters>(c), 100.0 * 3.1415 * 3.1415, 0.001);
}



TEST(Derived, Test1) {
    speed a = make_speed<meters, seconds>(10.0);
    timespan b = make_timespan<seconds>(2.0);
    length c = a * b;

    EXPECT_NEAR(get_length_as<meters>(c), 20.0, 0.001);
}

TEST(Derived, Test2) {
    speed a = make_speed<meters, seconds>(10.0, symbols::pi{});
    timespan b = make_timespan<seconds>(2.0);
    length c = a * b;

    EXPECT_NEAR(get_length_as<meters>(c), 20.0 * 3.1415, 0.001);
}


TEST(Derived, Test3) {
    speed a = make_speed<knots>(10.0);
    timespan b = make_timespan<hours>(2.0);
    length c = a * b;

    EXPECT_NEAR(get_length_as<nautical_miles>(c), 20.0, 0.001);
}

TEST(Derived, Test4) {
    speed a = make_speed<knots>(10.0, symbols::pi{}, std::ratio<1, 2>{});
    timespan b = make_timespan<hours>(2.0);
    length c = a * b;

    EXPECT_NEAR(get_length_as<nautical_miles>(c), 10.0 * 3.1415, 0.001);
}
