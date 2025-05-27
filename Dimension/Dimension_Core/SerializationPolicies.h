#ifndef DIMENSION_SERIALIZATION_POLICIES_H
#define DIMENSION_SERIALIZATION_POLICIES_H

#include <cstring>

#include "StringLiteral.h"
#include "UnitSimplifier.h"
#include <vector>

namespace Dimension
{

   // Forward declaration
   template<are_unit_exponents... Units>
   class BaseDimension;

   /// @brief Validate that an input buffer's tag matches the hashed value corresponding to the type
   /// @tparam NumTuple Numerator tuple
   /// @tparam DenTuple Denominator tuple
   /// @tparam InputIt Iterator type
   /// @tparam HashPolicy Hash policy implementation 
   /// @param in iterator to serialized buffer
   /// @return bool indicating if tag is valid for this type hash
   template <typename NumTuple, typename DenTuple, typename InputIt, typename HashPolicy>
   bool validateTag(InputIt in)
   {
      if constexpr(!std::is_void_v<typename HashPolicy::tag_type::type>)
      {
         typename HashPolicy::tag_type::type tag;
         std::memcpy(&tag, &*in, HashPolicy::tag_size);

         constexpr typename HashPolicy::tag_type expected_tag = TypeTagHelper<NumTuple, DenTuple, HashPolicy>::value();
         return (tag == expected_tag.get());
      }
      else
      {
         return true;
      }
   }

   /// @brief Default serialization policy
   /// @tparam HashPolicy Policy used to hash the type into a tag to serialize alongside the data
   template<typename HashPolicy>
   struct DefaultSerializationPolicy
   {
   private:
      template <typename NumTuple, typename DenTuple, typename OutputIt, typename BufferSizeType>
      static void serialize_impl(OutputIt out, const BaseDimension<NumTuple, DenTuple>& obj)
      {
         if constexpr(!std::is_void_v<typename HashPolicy::tag_type::type>)
         {
            constexpr auto tagData = TypeTagHelper<NumTuple, DenTuple, HashPolicy>::value().get();
            std::memcpy(&*out, &tagData, HashPolicy::tag_size);
            out += (HashPolicy::tag_size / sizeof(BufferSizeType));
         }

         // Write the double scalar
         PrecisionType temp = obj.template GetVal<NumTuple, DenTuple>();
         std::memcpy(&*out, &temp, sizeof(PrecisionType));
      }

      template <typename NumTuple, typename DenTuple, typename InputIt, typename BufferSizeType>
      static PrecisionType deserialize_impl(InputIt in)
      {
         if (!validateTag<NumTuple, DenTuple, InputIt, HashPolicy>(in))
         {
            throw std::invalid_argument("Type tag mismatch during deserialization");
         }

         in += (HashPolicy::tag_size / sizeof(BufferSizeType));

         double val;
         std::memcpy(&val, &*in, sizeof(val));

         return val;
      }

   public:
      /// @brief serialize a BaseDimension object into a passed buffer
      /// @tparam NumTuple numerator tuple to template Serializer on
      /// @tparam DenTuple denominator tuple to template Serializer on
      /// @tparam OutputBuf The buffer type
      /// @param out The buffer to serialize into
      /// @param obj The object to serialize
      template <typename NumTuple, typename DenTuple, typename OutputBuf>
      static void serialize(OutputBuf& out, const BaseDimension<NumTuple, DenTuple>& obj)
      {
         constexpr size_t required_size = HashPolicy::tag_size + sizeof(PrecisionType);

         if (out.size() * sizeof(typename OutputBuf::value_type) < required_size)
         {
            throw std::invalid_argument("Buffer size is too small. Must be at least " + std::to_string(required_size) + " bytes with these parameters.");
         }
         serialize_impl<NumTuple, DenTuple, decltype(out.begin()), typename OutputBuf::value_type>(out.begin(), obj);
      }

      /// @brief serialize a BaseDimension object and return the buffer
      /// @tparam NumTuple numerator tuple to template Serializer on
      /// @tparam DenTuple denominator tuple to template Serializer on
      /// @tparam OutputBuf The buffer type
      /// @param obj The object to serialize
      /// @return A new buffer populated with data from serializing obj
      template <typename NumTuple, typename DenTuple, typename OutputBuf = std::vector<uint8_t>>
      static OutputBuf serialize(const BaseDimension<NumTuple, DenTuple>& obj)
      {
         // Generate the unique type tag based on `NumTuple` and `DenTuple`
         OutputBuf out;
         out.resize(HashPolicy::tag_size + sizeof(PrecisionType));

         serialize_impl<NumTuple, DenTuple, decltype(out.begin()), typename OutputBuf::value_type>(out.begin(), obj);

         return out;
      }

      /// @brief deserialize a buffer and return the corresponding object
      /// @tparam NumTuple numerator tuple to template Serializer on
      /// @tparam DenTuple denominator tuple to template Serializer on
      /// @tparam InputBuf The buffer type
      /// @param in The buffer to deserialize
      /// @return A new BaseDimension object populated with data from deserializing input buffer
      template <typename NumTuple, typename DenTuple, typename InputBuf>
      static BaseDimension<NumTuple, DenTuple> deserialize(const InputBuf& in)
      {
         PrecisionType val = deserialize_impl<NumTuple, DenTuple, decltype(in.begin()), typename InputBuf::value_type>(in.begin());
         return BaseDimension<NumTuple, DenTuple>(val);
      }

      /// @brief deserialize a buffer and update the input object
      /// @tparam NumTuple numerator tuple to template Serializer on
      /// @tparam DenTuple denominator tuple to template Serializer on
      /// @tparam InputBuf The buffer type
      /// @param in The buffer to deserialize
      /// @param obj The object to deserialize into
      template <typename NumTuple, typename DenTuple, typename InputBuf>
      static void deserialize(const InputBuf& in, BaseDimension<NumTuple, DenTuple>& obj)
      {
         PrecisionType val = deserialize_impl<NumTuple, DenTuple, decltype(in.begin()), typename InputBuf::value_type>(in.begin());
         obj.template SetVal<NumTuple, DenTuple>(val);
      }

   };


} // end Dimension

#endif // DIMENSION_SERIALIZATION_POLICIES_H
