#ifndef DIMENSION_HASHING_H
#define DIMENSION_HASHING_H

#include <cstdint> // for std::uint#_t

#include "StringLiteral.h"

namespace Dimension
{

   /// @brief Wrapper for tags to allow return of void types
   /// @details Value is stored in value and size represents size in bytes
   /// @tparam T Type to wrap
   template <typename T>
   struct TagWrapper
   {
      using type = T;
      T value;
      static constexpr size_t size = sizeof(T);
      constexpr TagWrapper(const T& v) : value(v) {}
      constexpr T get() const { return value; }
   };

   /// @brief Wrapper for tags, specialization for void
   /// @details size is zero
   template <>
   struct TagWrapper<void>
   {
      using type = void;
      static constexpr size_t size = 0;
      constexpr void get() const noexcept {} // No-op
   };

   /// @brief No-op hashing policy
   /// @details Satisfies constraints of a hashing policy without doing
   ///   any hashing. Results in void tag type.
   struct NoHash
   {
      using tag_type = TagWrapper<void>;

      static constexpr size_t tag_size = tag_type::size;

      // FNV-1a hash function for `StringLiteral`
      template <std::size_t N>
      static constexpr tag_type hash_string_literal(const StringLiteral<N>&) { return tag_type(); } // unused parameter for no-op signature consistency
   };

   /// @brief Hashing policy using FNV-1a algorithm resulting in 32-bit tag
   /// @details This method hashes a string to a 32-bit value.
   ///   COLLISIONS ARE POSSIBLE WITH THIS METHOD
   struct FNV_1a_32Bit
   {
      using tag_type = TagWrapper<std::uint32_t>;

      static constexpr size_t tag_size = tag_type::size;

      // FNV-1a hash function for `StringLiteral`
      template <std::size_t N>
      static constexpr tag_type hash_string_literal(const StringLiteral<N>& str)
      {
         std::uint32_t hash = 0x811C9DC5;
         for (char c : str.value)
         {
            hash ^= static_cast<std::uint32_t>(c);
            hash *= 0x01000193;
         }
         return tag_type(hash);
      }
   };

   /// @brief Provide a post-hashed tag based on string representation of data
   /// @details Sorts the string representation of each item in both the numerator and denominator
   ///   then concatenates the two. This string is the input for hashing.
   /// @tparam NumTuple Numerator tuple
   /// @tparam DenTuple Denominator tuple
   /// @tparam HashPolicy Policy providing a hash_string_literal method for hashing
   template <typename NumTuple, typename DenTuple, typename HashPolicy>
   struct TypeTagHelper 
   {
      using SortedNumTuple = tuple_bubble_sort<NumTuple>::type;
      using SortedDenTuple = tuple_bubble_sort<DenTuple>::type;

      static constexpr auto NumTupleString = TupleStringConcat<SortedNumTuple>::value();
      static constexpr auto DenTupleString = TupleStringConcat<SortedDenTuple>::value();

      static constexpr auto FullString = concat(NumTupleString, DenTupleString);

      static constexpr HashPolicy::tag_type value()
      {
         return HashPolicy::template hash_string_literal(FullString);
      }
   };

} // end Dimension

#endif // DIMENSION_HASHING_H
