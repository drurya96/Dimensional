#include "DimensionTest.h"

#include "Dimension_Core/streaming/Stream.h"

#include <iostream>
#include <sstream>

using namespace dimension;

template<typename Policy, typename... Units>
void ValidateWrapperWithReturns()
{
    base_dimension<Units...> obj(25.0);
    auto buffer = serialize<decltype(obj), Policy>(obj);

    auto result = deserialize<base_dimension<Units...>, decltype(buffer), Policy>(buffer);
    EXPECT_NEAR((get_dimension_as<Units...>(result)), 25.0, TOLERANCE);
}

template<typename... Units>
void ValidateAll()
{
    ValidateWrapperWithReturns<ExactTagPolicy, Units...>();
    ValidateWrapperWithReturns<RawValuePolicy, Units...>();
}


TEST(Serialization, TestSingleNum)
{
    ValidateAll<unit_exponent<feet>>();
}

TEST(Serialization, TestSingleInverse)
{
    ValidateAll<unit_exponent<seconds, -1>>();
}

TEST(Serialization, TestOneEach)
{
    ValidateAll<unit_exponent<grams>, unit_exponent<radians, -1>>();
}

TEST(Serialization, TestMultipleEach)
{
    ValidateAll<unit_exponent<grams>, unit_exponent<meters>, unit_exponent<radians, -1>, unit_exponent<seconds, -1>>();
}

TEST(Serialization, TestShouldCancel)
{
    ValidateAll<unit_exponent<meters>, unit_exponent<meters, -1>>();
}
