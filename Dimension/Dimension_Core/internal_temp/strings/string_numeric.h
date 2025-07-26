#ifndef DIMENSIONAL_STRING_NUMERIC_H
#define DIMENSIONAL_STRING_NUMERIC_H

#include <array> // For std::array

#include "string_literal.h"

namespace dimension
{

   // TODO: This should probably live inside string_literal...
   //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
   //  helpers that manufacture string_literal<K> directly
   //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
   template<char C>
   constexpr auto lit()                      // "x"
   {
      return dimension::string_literal<2>({ C, '\0' });
   }

   /* decimal digit sequence of a positive integer, in reverse order */
   template<std::size_t N>
   constexpr auto digits_rev()
   {
      if constexpr (N < 10)
         return lit<'0' + N>();
      else
      {
         constexpr auto tail = digits_rev<N / 10>();
         return dimension::concat(lit<'0' + (N % 10)>(), tail);
      }
   }

   /* forward-order decimal of a positive integer */
   template<std::size_t N>
   constexpr auto digits_fwd()
   {
      constexpr auto rev = digits_rev<N>();
      // reverse the character array except null
      constexpr std::size_t len = rev.size - 1;
      std::array<char, len + 1> tmp{};
      for (std::size_t i = 0; i < len; ++i)
         tmp[i] = rev.value[len - 1 - i];
      return dimension::string_literal<len + 1>(tmp);
   }

   /* signed integer to string_literal */
   template<int V>
   constexpr auto int_literal()
   {
      if constexpr (V < 0)
         return dimension::concat(lit<'-'>(), digits_fwd<-V>());
      else
         return digits_fwd<V>();
   }

   /* ratio to string_literal  (Num / Den) */
   template<int Num, int Den>
   constexpr auto ratio_literal()
   {
      if constexpr (Den == 1)
         return int_literal<Num>();
      else
         return dimension::concat(
                     dimension::concat(int_literal<Num>(), lit<'/' >()),
                     int_literal<Den>());
   }

}

#endif // DIMENSIONAL_STRING_NUMERIC_H
