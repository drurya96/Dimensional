#ifndef DIMENSION_SERIALIZATION_H
#define DIMENSION_SERIALIZATION_H

#include "exact_tag_policy.h" // Included here since its the default

namespace dimension
{

   /// @brief Serializer to handle serialization and deserialization
   /// @details Policy-based serialization API allowing user-defined serialization logic
   /// @tparam NumTuple The numerator tuple for use in serialization
   /// @tparam DenTuple The denominator tuple for use in serialization
   /// @tparam Policy The serialization policy. This policy must minimally provide the serialize and/or deserialize
   ///   methods to use. Generally, the same serialization policy should be used for serialize and deserialize.
   ///   The defualt policy is itself policy-based, where it is templated on a hashing policy.
   template <is_base_dimension Dim, typename Policy = ExactTagPolicy>
   struct Serializer
   {
      /// @brief serialize a base_dimension object and return the buffer
      /// @tparam OutputBuf The buffer type
      /// @param out The buffer to serialize
      /// @return A new buffer populated with data from serializing obj
      static auto serialize(const Dim& obj)
      {
         return Policy::template serialize<Dim>(obj);
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

   };

   /// @brief serialize a base_dimension object and return the buffer
   /// @tparam NumTuple numerator tuple to template Serializer on
   /// @tparam DenTuple denominator tuple to template Serializer on
   /// @tparam OutputBuf The buffer type
   /// @tparam Policy Serialization policy to template Serializer on
   /// @param obj The object to serialize
   /// @return A new buffer populated with data from serializing obj
   template <is_base_dimension Dim, typename Policy = ExactTagPolicy>
   auto serialize(const Dim& obj)
   {
      return Serializer<Dim, Policy>::serialize(obj);
   }

   /// @brief deserialize a buffer and return the corresponding object
   /// @tparam NumTuple numerator tuple to template Serializer on
   /// @tparam DenTuple denominator tuple to template Serializer on
   /// @tparam InputBuf The buffer type
   /// @tparam Policy Serialization policy to template Serializer on
   /// @param in The buffer to deserialize
   /// @return A new base_dimension object populated with data from deserializing input buffer
   template <is_base_dimension Dim, typename InputBuf, typename Policy = ExactTagPolicy>
   Dim deserialize(const InputBuf& in)
   {
      return Serializer<Dim, Policy>::deserialize(in);
   }

} // end Dimension

#endif // DIMENSION_SERIALIZATION_H
