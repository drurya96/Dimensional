#ifndef DIMENSION_STRING_LITERAL_H
#define DIMENSION_STRING_LITERAL_H

#include <array> // For std::array
#include <algorithm> // For std::copy_n, std::min
#include <ostream> // For std::ostream
#include <tuple> // For std::tuple and related operations

namespace Dimension
{
   
   template<size_t N>
   struct StringLiteral
   {
   public:
      static constexpr size_t size = N;
      std::array<char, N> value;

      constexpr StringLiteral(const char (&str)[N]) {
         std::copy_n(str, N, value.begin());
      }

      constexpr StringLiteral() : value{} {}
      constexpr StringLiteral(const std::array<char, N>& arr) : value(arr) {}
   };


   template<size_t N>
   std::ostream& operator<<(std::ostream& os, const StringLiteral<N>& literal)
   {
      return os.write(literal.value.data(), N - 1);
   }

   template <size_t N1, size_t N2>
   constexpr bool operator<(const StringLiteral<N1>& lhs, const StringLiteral<N2>& rhs) {
      size_t min_size = std::min(N1, N2);
      for (size_t i = 0; i < min_size; ++i) {
         if (lhs.value[i] < rhs.value[i]) {
               return true;
         }
         if (lhs.value[i] > rhs.value[i]) {
               return false;
         }
      }
      // If all characters are equal up to the shortest length, the shorter string is considered "less".
      return N1 < N2;
   }

   // Concatenate two StringLiteral objects
   template <size_t N1, size_t N2>
   constexpr StringLiteral<N1 + N2 - 1> concat(const StringLiteral<N1>& lhs, const StringLiteral<N2>& rhs) {
      std::array<char, N1 + N2 - 1> combined = {};  // Combined array size (excluding double null terminator)

      // Copy lhs without its null terminator
      std::copy_n(lhs.value.begin(), N1 - 1, combined.begin());

      // Copy rhs including its null terminator
      std::copy_n(rhs.value.begin(), N2, combined.begin() + N1 - 1);

      // Use the private constructor of StringLiteral to create the result
      return StringLiteral<N1 + N2 - 1>(combined);
   }



   template <typename Tuple, std::size_t Index = 0>
   struct TupleStringConcat
   {
      static constexpr auto value() {
         if constexpr (std::tuple_size_v<Tuple> == 0)
         {
            return "";
         }
         else if constexpr (Index < std::tuple_size_v<Tuple> - 1)
         {
            return concat(concat(std::tuple_element_t<Index, Tuple>::qualifiedName, StringLiteral<2>("|")), TupleStringConcat<Tuple, Index + 1>::value());
         } else 
         {
            return std::tuple_element_t<Index, Tuple>::qualifiedName; // Base case for when Index == std::tuple_size_v<Tuple>
         }
      }
   };

   } // end Dimension

#endif // DIMENSION_STRING_LITERAL_H