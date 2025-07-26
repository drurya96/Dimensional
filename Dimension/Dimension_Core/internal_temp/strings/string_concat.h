#ifndef DIMENSIONAL_STRING_CONCAT_H
#define DIMENSIONAL_STRING_CONCAT_H

#include <array> // For std::array
#include <algorithm> // For std::copy_n
#include <tuple> // For std::tuple and related operations

#include "string_literal.h"

namespace dimension
{

   template <typename...>
   struct ConcatHelper;

   template <size_t N>
   struct ConcatHelper<string_literal<N>> {
      static constexpr auto value(const string_literal<N>& s) {
         return s;
      }
   };

   template <size_t N1, size_t N2, typename... Rest>
   struct ConcatHelper<string_literal<N1>, string_literal<N2>, Rest...> {
      static constexpr auto value(const string_literal<N1>& lhs, const string_literal<N2>& rhs, const Rest&... rest) {
         constexpr size_t combined_size = N1 + N2 - 1;
         std::array<char, combined_size> combined{};

         std::copy_n(lhs.value.begin(), N1 - 1, combined.begin());
         std::copy_n(rhs.value.begin(), N2, combined.begin() + N1 - 1);

         return ConcatHelper<string_literal<combined_size>, Rest...>::value(string_literal<combined_size>(combined), rest...);
      }
   };

   template <typename... Strings>
   constexpr auto concat(const Strings&... strs) {
      return ConcatHelper<Strings...>::value(strs...);
   }

   /// @brief Concatenate a tuple of various string_literals
   /// @tparam Tuple tuple of string_literals
   /// @tparam Index Current index for recussive calls
   template <typename Tuple, std::size_t Index = 0>
   struct TupleStringConcat
   {
      static constexpr auto value() {
         if constexpr (std::tuple_size_v<Tuple> == 0)
         {
            return string_literal<1>("");
         }
         else if constexpr (Index < std::tuple_size_v<Tuple> - 1)
         {
            return concat(concat(std::tuple_element_t<Index, Tuple>::qualifiedName, string_literal<2>("|")), TupleStringConcat<Tuple, Index + 1>::value());
         } else 
         {
            return std::tuple_element_t<Index, Tuple>::qualifiedName; // Base case for when Index == std::tuple_size_v<Tuple>
         }
      }
   };

}

#endif // DIMENSIONAL_STRING_CONCAT_H
