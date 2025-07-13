#include "DimensionTest.h"

#include "Dimension_Core/Stream.h"

#include <iostream>
#include <sstream>

using namespace dimension;

template<typename... Units>
void ValidateWrapperWithReturns()
{
    base_dimension<Units...> obj(25.0);
    auto buffer = serialize(obj);

    EXPECT_EQ(buffer.size(), sizeof(uint32_t) + sizeof(double));

    auto result = deserialize<base_dimension<Units...>, decltype(buffer)>(buffer);
    EXPECT_NEAR((get_dimension_as<Units...>(result)), 25.0, TOLERANCE);
}

template<typename BufferType, typename... Units>
void ValidateWrapperWithRefs()
{

    using elementType = BufferType::value_type;
    constexpr size_t requiredSize = (sizeof(uint32_t) + sizeof(PrecisionType) + sizeof(elementType) - 1) / sizeof(elementType);

    BufferType buffer;
    buffer.resize(requiredSize);

    base_dimension<Units...> obj(25.0);
    serialize(buffer, obj);

    EXPECT_EQ(buffer.size() * sizeof(elementType), sizeof(uint32_t) + sizeof(PrecisionType));

    //base_dimension<Units...> result;
    auto result = deserialize<base_dimension<Units...>>(buffer);

    std::cout << "Deserialized Value: " << result << std::endl;

    EXPECT_NEAR((get_dimension_as<Units...>(result)), 25.0, TOLERANCE);

}

template<typename... Units>
void ValidateNoHash()
{

    using TestSerializer = Serializer<base_dimension<Units...>, DefaultSerializationPolicy<NoHash>>;

    base_dimension<Units...> obj(25.0);
    auto buffer = TestSerializer::serialize(obj);

    EXPECT_EQ(buffer.size(), sizeof(PrecisionType));

    auto result = TestSerializer::deserialize(buffer);
    EXPECT_NEAR((get_dimension_as<Units...>(result)), 25.0, TOLERANCE);
}



template<typename... Units>
void ValidateAll()
{
    ValidateWrapperWithReturns<Units...>();

    ValidateWrapperWithRefs<std::vector<uint8_t>, Units...>();
    ValidateWrapperWithRefs<std::vector<uint16_t>, Units...>();
    ValidateWrapperWithRefs<std::vector<uint32_t>, Units...>();

    // In the current state, buffer word size cannot exceed hashed-tag size.
    // Fix in future serializatio updates.

    ValidateNoHash<Units...>();
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
