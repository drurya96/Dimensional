#ifndef DIMENSION_HASHING_H
#define DIMENSION_HASHING_H

#include <cstdint> // for std::uint#_t

#include "StringLiteral.h"

namespace Dimension
{

   // FNV-1a hash function for `StringLiteral`
   template <std::size_t N>
   constexpr std::uint32_t hash_string_literal(const StringLiteral<N>& str) {
      std::uint32_t hash = 0x811C9DC5;
      for (char c : str.value) {
         hash ^= static_cast<std::uint32_t>(c);
         hash *= 0x01000193;
      }
      return hash;
   }



   template <typename NumTuple, typename DenTuple>
   struct TypeTagHelper 
   {
      using SortedNumTuple = tuple_bubble_sort<NumTuple>::type;
      using SortedDenTuple = tuple_bubble_sort<DenTuple>::type;

      static constexpr auto NumTupleString = TupleStringConcat<SortedNumTuple>::value();
      static constexpr auto DenTupleString = TupleStringConcat<SortedDenTuple>::value();

      static constexpr auto FullString = concat(NumTupleString, DenTupleString);


      static constexpr std::uint32_t value = hash_string_literal(FullString);
   };

} // end Dimension

#endif // DIMENSION_HASHING_H