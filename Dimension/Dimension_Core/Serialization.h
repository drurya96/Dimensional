#ifndef DIMENSION_SERIALIZATION_H
#define DIMENSION_SERIALIZATION_H

#include <cstring>
#include <vector>

#include "StringLiteral.h"
#include "SerializationPolicies.h"


namespace dimension
{

   /// @brief Serializer to handle serialization and deserialization
   /// @details Policy-based serialization API allowing user-defined serialization logic
   /// @tparam NumTuple The numerator tuple for use in serialization
   /// @tparam DenTuple The denominator tuple for use in serialization
   /// @tparam Policy The serialization policy. This policy must minimally provide the serialize and/or deserialize
   ///   methods to use. Generally, the same serialization policy should be used for serialize and deserialize.
   ///   The defualt policy is itself policy-based, where it is templated on a hashing policy.
   template <is_base_dimension Dim, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
   struct Serializer
   {
      /// @brief serialize a base_dimension object into a passed buffer
      /// @tparam OutputBuf The buffer type
      /// @param out The buffer to serialize into
      /// @param obj The object to serialize
      template<typename OutputBuf>
      static void serialize(OutputBuf& out, const Dim& obj)
      {
         return Policy::template serialize<Dim, OutputBuf>(out, obj);
      }

      /// @brief serialize a base_dimension object and return the buffer
      /// @tparam OutputBuf The buffer type
      /// @param out The buffer to serialize
      /// @return A new buffer populated with data from serializing obj
      template<typename OutputBuf = std::vector<uint8_t>>
      static OutputBuf serialize(const Dim& obj)
      {
         return Policy::template serialize<Dim, OutputBuf>(obj);
      }

      /// @brief deserialize a buffer and return the corresponding object
      /// @tparam InputBuf The buffer type
      /// @param in The buffer to deserialize
      /// @return A new base_dimension object populated with data from deserializing input buffer
      template <typename InputBuf>
      static Dim deserialize(const InputBuf& in)
      {
         return Policy::template deserialize<Dim, InputBuf>(in);
      }

      /// @brief deserialize a buffer and update the input object
      /// @tparam InputBuf The buffer type
      /// @param in The buffer to deserialize
      /// @param obj The object to update
      template <typename InputBuf>
      static void deserialize(const InputBuf& in, Dim& obj)
      {
         return Policy::template deserialize<Dim, InputBuf>(in, obj);
      }
   };

   /// @brief serialize a base_dimension object into a passed buffer
   /// @tparam NumTuple numerator tuple to template Serializer on
   /// @tparam DenTuple denominator tuple to template Serializer on
   /// @tparam OutputBuf The buffer type
   /// @tparam Policy Serialization policy to template Serializer on
   /// @param out The buffer to serialize into
   /// @param obj The object to serialize
   template <is_base_dimension Dim, typename OutputBuf, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
   void serialize(OutputBuf& out, const Dim& obj)
   {
      Serializer<Dim, Policy>::serialize(out, obj);
   }

   /// @brief serialize a base_dimension object and return the buffer
   /// @tparam NumTuple numerator tuple to template Serializer on
   /// @tparam DenTuple denominator tuple to template Serializer on
   /// @tparam OutputBuf The buffer type
   /// @tparam Policy Serialization policy to template Serializer on
   /// @param obj The object to serialize
   /// @return A new buffer populated with data from serializing obj
   template <is_base_dimension Dim, typename OutputBuf = std::vector<uint8_t>, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
   OutputBuf serialize(const Dim& obj)
   {
      return Serializer<Dim, Policy>::template serialize<OutputBuf>(obj);
   }

   /// @brief deserialize a buffer and return the corresponding object
   /// @tparam NumTuple numerator tuple to template Serializer on
   /// @tparam DenTuple denominator tuple to template Serializer on
   /// @tparam InputBuf The buffer type
   /// @tparam Policy Serialization policy to template Serializer on
   /// @param in The buffer to deserialize
   /// @return A new base_dimension object populated with data from deserializing input buffer
   template <is_base_dimension Dim, typename InputBuf, typename Policy = DefaultSerializationPolicy<FNV_1a_32Bit>>
   Dim deserialize(const InputBuf& in)
   {
      return Serializer<Dim, Policy>::deserialize(in);
   }

} // end Dimension

#endif // DIMENSION_SERIALIZATION_H
