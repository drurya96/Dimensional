#include "DimensionTest.h"

#include "Dimension_Core/Stream.h"

#include <iostream>
#include <sstream>

using namespace dimension;

template<typename NumTuple, typename DenTuple>
void ValidateWrapperWithReturns()
{
    base_dimension<NumTuple, DenTuple> obj(25.0);
    auto buffer = serialize(obj);

    EXPECT_EQ(buffer.size(), sizeof(uint32_t) + sizeof(PrecisionType));

    auto result = deserialize<NumTuple, DenTuple, decltype(buffer)>(buffer);
    EXPECT_NEAR((result.template GetVal<NumTuple, DenTuple>()), 25.0, TOLERANCE);
}

template<typename NumTuple, typename DenTuple, typename BufferType>
void ValidateWrapperWithRefs()
{

    using elementType = BufferType::value_type;
    constexpr size_t requiredSize = (sizeof(uint32_t) + sizeof(PrecisionType) + sizeof(elementType) - 1) / sizeof(elementType);

    BufferType buffer;
    buffer.resize(requiredSize);

    base_dimension<NumTuple, DenTuple> obj(25.0);
    serialize(buffer, obj);

    EXPECT_EQ(buffer.size() * sizeof(elementType), sizeof(uint32_t) + sizeof(PrecisionType));

    base_dimension<NumTuple, DenTuple> result;
    deserialize<NumTuple, DenTuple>(buffer, result);

    EXPECT_NEAR((result.template GetVal<NumTuple, DenTuple>()), 25.0, TOLERANCE);

}

template<typename NumTuple, typename DenTuple>
void ValidateNoHash()
{

    using TestSerializer = Serializer<NumTuple, DenTuple, DefaultSerializationPolicy<NoHash>>;

    base_dimension<NumTuple, DenTuple> obj(25.0);
    auto buffer = TestSerializer::serialize(obj);

    EXPECT_EQ(buffer.size(), sizeof(PrecisionType));

    auto result = TestSerializer::deserialize(buffer);
    EXPECT_NEAR((result.template GetVal<NumTuple, DenTuple>()), 25.0, TOLERANCE);
}



template<typename NumTuple, typename DenTuple>
void ValidateAll()
{
    ValidateWrapperWithReturns<NumTuple, DenTuple>();

    ValidateWrapperWithRefs<NumTuple, DenTuple, std::vector<uint8_t>>();
    ValidateWrapperWithRefs<NumTuple, DenTuple, std::vector<uint16_t>>();
    ValidateWrapperWithRefs<NumTuple, DenTuple, std::vector<uint32_t>>();

    // In the current state, buffer word size cannot exceed hashed-tag size.
    // Fix in future serializatio updates.

    ValidateNoHash<NumTuple, DenTuple>();
}


TEST(Serialization, TestSingleNum)
{
    using NumTuple = std::tuple<Feet>;
    using DenTuple = std::tuple<>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST(Serialization, TestSingleInverse)
{
    using NumTuple = std::tuple<>;
    using DenTuple = std::tuple<seconds>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST(Serialization, TestOneEach)
{
    using NumTuple = std::tuple<Grams>;
    using DenTuple = std::tuple<radians>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST(Serialization, TestMultipleEach)
{
    using NumTuple = std::tuple<Grams, meters>;
    using DenTuple = std::tuple<radians, seconds>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST(Serialization, TestShouldCancel)
{
    using NumTuple = std::tuple<meters>;
    using DenTuple = std::tuple<meters>;

    ValidateAll<NumTuple, DenTuple>();
}
