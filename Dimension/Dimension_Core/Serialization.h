#ifndef DIMENSION_SERIALIZATION_H
#define DIMENSION_SERIALIZATION_H

#include <cstring>

#include "StringLiteral.h"
#include "UnitSimplifier.h"
#include "SerializationPolicies.h"
#include <vector>

namespace Dimension
{

   /// @brief Serializer to handle serialization and deserialization
   /// @details Policy-based serialization API allowing user-defined serialization logic
   /// @tparam NumTuple The numerator tuple for use in serialization
   /// @tparam DenTuple The denominator tuple for use in serialization
   /// @tparam Policy The serialization policy. This policy must minimally provide the serialize and/or deserialize
   ///   methods to use. Generally, the same serialization policy should be used for serialize and deserialize.
   ///   The defualt policy is itself policy-based, where it is templated on a hashing policy.
   template <typename NumTuple, typename DenTuple, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
   struct Serializer
   {
      /// @brief serialize a BaseDimension object into a passed buffer
      /// @tparam OutputBuf The buffer type
      /// @param out The buffer to serialize into
      /// @param obj The object to serialize
      template<typename OutputBuf>
      static void serialize(OutputBuf& out, const BaseDimension<NumTuple, DenTuple>& obj)
      {
         return Policy::template serialize<NumTuple, DenTuple, OutputBuf>(out, obj);
      }

      /// @brief serialize a BaseDimension object and return the buffer
      /// @tparam OutputBuf The buffer type
      /// @param out The buffer to serialize
      /// @return A new buffer populated with data from serializing obj
      template<typename OutputBuf = std::vector<uint8_t>>
      static OutputBuf serialize(const BaseDimension<NumTuple, DenTuple>& obj)
      {
         return Policy::template serialize<NumTuple, DenTuple, OutputBuf>(obj);
      }

      /// @brief deserialize a buffer and return the corresponding object
      /// @tparam InputBuf The buffer type
      /// @param in The buffer to deserialize
      /// @return A new BaseDimension object populated with data from deserializing input buffer
      template <typename InputBuf>
      static BaseDimension<NumTuple, DenTuple> deserialize(const InputBuf& in)
      {
         return Policy::template deserialize<NumTuple, DenTuple, InputBuf>(in);
      }

      /// @brief deserialize a buffer and update the input object
      /// @tparam InputBuf The buffer type
      /// @param in The buffer to deserialize
      /// @param obj The object to update
      template <typename InputBuf>
      static void deserialize(const InputBuf& in, BaseDimension<NumTuple, DenTuple>& obj)
      {
         return Policy::template deserialize<NumTuple, DenTuple, InputBuf>(in, obj);
      }
   };

   /// @brief serialize a BaseDimension object into a passed buffer
   /// @tparam NumTuple numerator tuple to template Serializer on
   /// @tparam DenTuple denominator tuple to template Serializer on
   /// @tparam OutputBuf The buffer type
   /// @tparam Policy Serialization policy to template Serializer on
   /// @param out The buffer to serialize into
   /// @param obj The object to serialize
   template <typename NumTuple, typename DenTuple, typename OutputBuf, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
   void serialize(OutputBuf& out, const BaseDimension<NumTuple, DenTuple>& obj)
   {
      Serializer<NumTuple, DenTuple, Policy>::serialize(out, obj);
   }

   /// @brief serialize a BaseDimension object and return the buffer
   /// @tparam NumTuple numerator tuple to template Serializer on
   /// @tparam DenTuple denominator tuple to template Serializer on
   /// @tparam OutputBuf The buffer type
   /// @tparam Policy Serialization policy to template Serializer on
   /// @param obj The object to serialize
   /// @return A new buffer populated with data from serializing obj
   template <typename NumTuple, typename DenTuple, typename OutputBuf = std::vector<uint8_t>, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
   OutputBuf serialize(const BaseDimension<NumTuple, DenTuple>& obj)
   {
      return Serializer<NumTuple, DenTuple, Policy>::template serialize<OutputBuf>(obj);
   }

   /// @brief deserialize a buffer and return the corresponding object
   /// @tparam NumTuple numerator tuple to template Serializer on
   /// @tparam DenTuple denominator tuple to template Serializer on
   /// @tparam InputBuf The buffer type
   /// @tparam Policy Serialization policy to template Serializer on
   /// @param in The buffer to deserialize
   /// @return A new BaseDimension object populated with data from deserializing input buffer
   template <typename NumTuple, typename DenTuple, typename InputBuf, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
   BaseDimension<NumTuple, DenTuple> deserialize(const InputBuf& in)
   {
      return Serializer<NumTuple, DenTuple, Policy>::deserialize(in);
   }

   /// @brief deserialize a buffer and update the input object
   /// @tparam NumTuple numerator tuple to template Serializer on
   /// @tparam DenTuple denominator tuple to template Serializer on
   /// @tparam InputBuf The buffer type
   /// @tparam Policy Serialization policy to template Serializer on
   /// @param in The buffer to deserialize
   /// @param obj The object to deserialize into
   template <typename NumTuple, typename DenTuple, typename InputBuf, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
   void deserialize(const InputBuf& in, BaseDimension<NumTuple, DenTuple>& obj)
   {
      Serializer<NumTuple, DenTuple, Policy>::deserialize(in, obj);
   }

} // end Dimension

#endif // DIMENSION_SERIALIZATION_H
