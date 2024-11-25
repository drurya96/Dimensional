#include "DimensionTest.h"

#include "BaseDimension.h"

#include "TimeDimension.h"
#include "LengthDimension.h"
#include "MassDimension.h"
#include "AngleDimension.h"
#include "SpeedDimension.h"

#include "Dimension_Core/Stream.h"

#include <iostream>
#include <sstream>

using namespace Dimension;

template<typename NumTuple, typename DenTuple>
void ValidateWrapperWithReturns()
{
    BaseDimension<NumTuple, DenTuple> obj(25.0);
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

    BaseDimension<NumTuple, DenTuple> obj(25.0);
    serialize(buffer, obj);

    EXPECT_EQ(buffer.size() * sizeof(elementType), sizeof(uint32_t) + sizeof(PrecisionType));

    BaseDimension<NumTuple, DenTuple> result;
    deserialize<NumTuple, DenTuple>(buffer, result);

    EXPECT_NEAR((result.template GetVal<NumTuple, DenTuple>()), 25.0, TOLERANCE);

}

template<typename NumTuple, typename DenTuple>
void ValidateNoHash()
{

    using TestSerializer = Serializer<NumTuple, DenTuple, DefaultSerializationPolicy<NoHash>>;

    BaseDimension<NumTuple, DenTuple> obj(25.0);
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


TEST_F(DimensionTest, TestSingleNum)
{
    using NumTuple = std::tuple<Feet>;
    using DenTuple = std::tuple<>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST_F(DimensionTest, TestSingleInverse)
{
    using NumTuple = std::tuple<>;
    using DenTuple = std::tuple<Seconds>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST_F(DimensionTest, TestOneEach)
{
    using NumTuple = std::tuple<Grams>;
    using DenTuple = std::tuple<Radians>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST_F(DimensionTest, TestMultipleEach)
{
    using NumTuple = std::tuple<Grams, Meters>;
    using DenTuple = std::tuple<Radians, Seconds>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST_F(DimensionTest, TestShouldCancel)
{
    using NumTuple = std::tuple<Meters>;
    using DenTuple = std::tuple<Meters>;

    ValidateAll<NumTuple, DenTuple>();
}
