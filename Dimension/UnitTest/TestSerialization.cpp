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

template<typename Dim>
void ValidateWrapperWithReturns()
{
    Dim obj(25.0);
    auto buffer = serialize(obj);

    EXPECT_EQ(buffer.size(), sizeof(uint32_t) + sizeof(PrecisionType));

    auto result = deserialize<Dim, decltype(buffer)>(buffer);
    EXPECT_NEAR((result.template GetVal<typename Dim::NumTuple, typename Dim::DenTuple>()), 25.0, TOLERANCE);
}

template<typename Dim, typename BufferType>
void ValidateWrapperWithRefs()
{

    using elementType = BufferType::value_type;
    constexpr size_t requiredSize = (sizeof(uint32_t) + sizeof(PrecisionType) + sizeof(elementType) - 1) / sizeof(elementType);

    BufferType buffer;
    buffer.resize(requiredSize);

    Dim obj(25.0);
    serialize(buffer, obj);

    EXPECT_EQ(buffer.size() * sizeof(elementType), sizeof(uint32_t) + sizeof(PrecisionType));

    Dim result;
    deserialize<Dim>(buffer, result);

    EXPECT_NEAR((result.template GetVal<typename Dim::NumTuple, typename Dim::DenTuple>()), 25.0, TOLERANCE);

}


template<typename Dim>
void ValidateNoHash()
{

    using TestSerializer = Serializer<Dim, DefaultSerializationPolicy<NoHash>>;

    Dim obj(25.0);
    auto buffer = TestSerializer::serialize(obj);

    EXPECT_EQ(buffer.size(), sizeof(PrecisionType));

    auto result = TestSerializer::deserialize(buffer);
    EXPECT_NEAR((result.template GetVal<typename Dim::NumTuple, typename Dim::DenTuple>()), 25.0, TOLERANCE);
    //EXPECT_NEAR((result.GetVal<NumTuple, DenTuple>()), 25.0, TOLERANCE);
}



template<typename NumTuple, typename DenTuple>
void ValidateAll()
{
    using Dim = BaseDimension<NumTuple, DenTuple, SymbolicList<>>;
    ValidateWrapperWithReturns<Dim>();

    ValidateWrapperWithRefs<Dim, std::vector<uint8_t>>();
    ValidateWrapperWithRefs<Dim, std::vector<uint16_t>>();
    ValidateWrapperWithRefs<Dim, std::vector<uint32_t>>();

    // In the current state, buffer word size cannot exceed hashed-tag size.
    // Fix in future serializatio updates.

    ValidateNoHash<Dim>();
}



TEST(Serialization, TestSingleNum)
{
    using NumTuple = std::tuple<Feet>;
    using DenTuple = std::tuple<>;

    ValidateAll<NumTuple, DenTuple>();
}
/*

TEST(Serialization, TestSingleInverse)
{
    using NumTuple = std::tuple<>;
    using DenTuple = std::tuple<Seconds>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST(Serialization, TestOneEach)
{
    using NumTuple = std::tuple<Grams>;
    using DenTuple = std::tuple<Radians>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST(Serialization, TestMultipleEach)
{
    using NumTuple = std::tuple<Grams, Meters>;
    using DenTuple = std::tuple<Radians, Seconds>;

    ValidateAll<NumTuple, DenTuple>();
}

TEST(Serialization, TestShouldCancel)
{
    using NumTuple = std::tuple<Meters>;
    using DenTuple = std::tuple<Meters>;

    ValidateAll<NumTuple, DenTuple>();
}
*/
