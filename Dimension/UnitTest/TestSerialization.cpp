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



/*
template<typename T>
std::ostream& operator<<(std::ostream& os, const Length<T>& obj) {
    // Output to the stream
    os << getLength<T>(obj) << " [" << T::abbr << "]";
    return os;
}
*/



TEST_F(DimensionTest, TestStreamSerializer) {

   BaseDimension<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>> myLen(25);

   //std::stringstream sstream;

   //sstream << myLen;

   std::cout << myLen << std::endl;

   std::string myStr = to_string(myLen);

   std::cout << myStr << std::endl;

   std::cout << Meters::qualifiedName << std::endl;

    constexpr uint32_t hashed = hash_string_literal(Feet::qualifiedName);


    std::cout << hashed << std::endl;

    using TupType = typename BaseDimension<std::tuple<Meters, Feet, Meters>, std::tuple<Seconds, Seconds>>::NumTuple;
    std::cout << "Concatenated String: " << TupleStringConcat<TupType>::value() << std::endl;

    using SortedTuple = tuple_bubble_sort<TupType>::type;
    std::cout << "Concatenated String: " << TupleStringConcat<SortedTuple>::value() << std::endl;


    //std::vector<uint8_t> buffer;
    //buffer.resize(sizeof(uint32_t) + sizeof(PrecisionType));

    //myLen.serialize(buffer.begin());
    //Serializer<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>>::serialize(buffer.begin(), myLen);
    //serialize(buffer.begin(), myLen);

    //auto buffer = Serializer<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>>::serialize(myLen);
    auto buffer = serialize(myLen);


    std::cout << "Tag: " << TypeTagHelper<decltype(myLen)::NumTuple, decltype(myLen)::DenTuple>::value << std::endl;

    for (uint8_t byte : buffer)
    {
        std::cout << std::hex << static_cast<int>(byte) << ' ';
    }
    std::cout << std::endl;


    

    //BaseDimension<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>> deserializedObject = BaseDimension<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>>::deserialize(buffer.begin());
    //auto deserializedObject = BaseDimension<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>>::deserialize(buffer.begin());

    //BaseDimension<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>> deserializedObject = Serializer<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>>::deserialize(buffer.begin());
    auto deserializedObject2 = Serializer<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>>::deserialize(buffer.begin());
    std::cout << deserializedObject2 << std::endl;

    //auto deserializedObject = deserialize<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>, BufferType>(buffer.begin());


    BaseDimension<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>> deserializedObject3;
    Serializer<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>>::deserialize(buffer.begin(), deserializedObject3);
    std::cout << deserializedObject3 << std::endl;

    BaseDimension<std::tuple<Feet, Feet>, std::tuple<Seconds, Seconds>> deserializedObject4;
    deserialize(buffer.begin(), deserializedObject4);
    std::cout << deserializedObject4 << std::endl;


    




    //using t = UnitFromString<"Length::Meters">::type;

   //static_assert(std::is_same_v<t, void>);

   //ASSERT_EQ(sstream.str(), "25 [ft]");

   

}




TEST_F(DimensionTest, TestSetter) {

using namespace std;

BaseDimension<tuple<Meters>, tuple<Seconds>> speed(25.0);

speed.SetVal<tuple<Feet>, tuple<Minutes>>(25.0);

double res = speed.GetVal<tuple<Meters>, tuple<Seconds>>();

cout << res << endl;

}