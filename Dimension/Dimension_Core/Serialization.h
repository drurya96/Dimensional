#ifndef DIMENSION_SERIALIZATION_H
#define DIMENSION_SERIALIZATION_H

#include <cstring>

#include "StringLiteral.h"
#include "UnitSimplifier.h"
#include <vector>

namespace Dimension
{

   template<typename NumTupleT, typename DenTupleT>
   requires IsUnitTuplePair<NumTupleT, DenTupleT>
   class BaseDimension;

   struct DefaultSerializationPolicy
   {
      template <typename NumTuple, typename DenTuple, typename InputIt>
      static BaseDimension<NumTuple, DenTuple> deserialize(InputIt in)
      {
         // Read the tag and verify it matches the expected tag
         std::uint32_t tag;
         std::memcpy(&tag, &*in, sizeof(tag));
         in += sizeof(tag);

         constexpr std::uint32_t expected_tag = TypeTagHelper<NumTuple, DenTuple>::value;
         if (tag != expected_tag) {
            throw std::invalid_argument("Type tag mismatch during deserialization");
         }

         // Read the double value
         double val;
         std::memcpy(&val, &*in, sizeof(val));
         
         return BaseDimension<NumTuple, DenTuple>(val);
      }

      template <typename NumTuple, typename DenTuple, typename InputIt>
      static void deserialize(InputIt in, BaseDimension<NumTuple, DenTuple>& obj)
      {
         // Read the tag and verify it matches the expected tag
         std::uint32_t tag;
         std::memcpy(&tag, &*in, sizeof(tag));
         in += sizeof(tag);

         constexpr std::uint32_t expected_tag = TypeTagHelper<NumTuple, DenTuple>::value;
         if (tag != expected_tag) {
            throw std::invalid_argument("Type tag mismatch during deserialization");
         }

         // Read the double value
         double val;
         std::memcpy(&val, &*in, sizeof(val));
         
         //return BaseDimension<NumTuple, DenTuple>(val);
         obj.template SetVal<NumTuple, DenTuple>(val);
      }

      // Serialize to a binary buffer
      template <typename NumTuple, typename DenTuple, typename OutputIt>
      static void serialize(OutputIt out, const BaseDimension<NumTuple, DenTuple>& obj)
      {
         // Generate the unique type tag based on `NumTuple` and `DenTuple`
         constexpr auto tag = TypeTagHelper<NumTuple, DenTuple>::value;
         std::memcpy(&*out, &tag, sizeof(tag));
         out += sizeof(tag);

         // Write the double scalar
         PrecisionType temp = obj.template GetVal<NumTuple, DenTuple>();
         std::memcpy(&*out, &temp, sizeof(PrecisionType));
      }

      template <typename NumTuple, typename DenTuple, typename OutputIt = std::vector<uint8_t>>
      static OutputIt serialize(const BaseDimension<NumTuple, DenTuple>& obj)
      {
         // Generate the unique type tag based on `NumTuple` and `DenTuple`

         OutputIt outBuffer;
         outBuffer.reserve(sizeof(uint32_t) + sizeof(PrecisionType));
         auto out = outBuffer.begin();

         constexpr auto tag = TypeTagHelper<NumTuple, DenTuple>::value;
         std::memcpy(&*out, &tag, sizeof(tag));
         out += sizeof(tag);

         // Write the double scalar
         PrecisionType temp = obj.template GetVal<NumTuple, DenTuple>();
         std::memcpy(&*out, &temp, sizeof(PrecisionType));

         return outBuffer;
      }

   };

   template <typename NumTuple, typename DenTuple, typename Policy = DefaultSerializationPolicy>
   struct Serializer
   {
      // Deserialize from a binary buffer
      template <typename InputIt>
      static BaseDimension<NumTuple, DenTuple> deserialize(InputIt in)
      {
         return Policy::template deserialize<NumTuple, DenTuple, InputIt>(in);
      }

      template <typename InputIt>
      static void deserialize(InputIt in, BaseDimension<NumTuple, DenTuple>& obj)
      {
         return Policy::template deserialize<NumTuple, DenTuple, InputIt>(in, obj);
      }

      template<typename OutputIt>
      static void serialize(OutputIt out, const BaseDimension<NumTuple, DenTuple>& obj)
      {
         return Policy::template serialize<NumTuple, DenTuple, OutputIt>(out, obj);
      }

      template<typename OutputIt = std::vector<uint8_t>>
      static OutputIt serialize(const BaseDimension<NumTuple, DenTuple>& obj)
      {
         return Policy::template serialize<NumTuple, DenTuple, OutputIt>(obj);
      }
   };

   template <typename NumTuple, typename DenTuple, typename OutputIt, typename Policy = DefaultSerializationPolicy>
   void serialize(OutputIt out, const BaseDimension<NumTuple, DenTuple>& obj)
   {
      Serializer<NumTuple, DenTuple, Policy>::serialize(out, obj);
   }

   template <typename NumTuple, typename DenTuple, typename OutputIt = std::vector<uint8_t>, typename Policy = DefaultSerializationPolicy>
   OutputIt serialize(const BaseDimension<NumTuple, DenTuple>& obj)
   {
      return Serializer<NumTuple, DenTuple, Policy>::template serialize<OutputIt>(obj);
   }

   template <typename NumTuple, typename DenTuple, typename InputIt, typename Policy = DefaultSerializationPolicy>
   void deserialize(InputIt in, BaseDimension<NumTuple, DenTuple>& obj)
   {
      Serializer<NumTuple, DenTuple, Policy>::deserialize(in, obj);
   }


} // end Dimension

#endif // DIMENSION_SERIALIZATION_H