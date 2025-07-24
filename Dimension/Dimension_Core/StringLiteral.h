#ifndef DIMENSION_STRING_LITERAL_H
#define DIMENSION_STRING_LITERAL_H

#include <array> // For std::array
#include <algorithm> // For std::copy_n, std::min
#include <ostream> // For std::ostream
#include <tuple> // For std::tuple and related operations

#include <sstream>

namespace dimension
{
   
   /// @brief Struct allowing strings as template parameters
   /// @details Templating on StringLiteral allows using raw strings as template parameters
   /// @tparam N Size of string
   template<size_t N>
   struct StringLiteral
   {
   public:
      static constexpr size_t size = N;
      std::array<char, N> value;

      // StringLiteral is meant to be used as a wrapper for strings so they may be used in template parameters, implicit conversion is acceptable.
      // cppcheck-suppress noExplicitConstructor
      constexpr StringLiteral(const char (&str)[N]) {
         std::copy_n(str, N, value.begin());
      }

      constexpr StringLiteral() : value{} {}
      // StringLiteral is meant to be used as a wrapper for strings so they may be used in template parameters, implicit conversion is acceptable.
      // cppcheck-suppress noExplicitConstructor
      constexpr StringLiteral(const std::array<char, N>& arr) : value(arr) {}
   };

   /// @brief Stream operator from a StringLiteral into an ostream
   /// @tparam N Size of string
   /// @param os stream to write to
   /// @param literal StringLiteral to write
   /// @return reference to stream written
   template<size_t N>
   std::ostream& operator<<(std::ostream& os, const StringLiteral<N>& literal)
   {
      return os.write(literal.value.data(), N - 1);
   }

   /// @brief Less than operator comparing two string literals alphabetically
   /// @tparam N1 Size of first string
   /// @tparam N2 Size of second string
   /// @param lhs Object to compare
   /// @param rhs Object to compare to
   /// @return bool indicating whether lhs is less than rhs
   template <size_t N1, size_t N2>
   constexpr bool operator<(const StringLiteral<N1>& lhs, const StringLiteral<N2>& rhs) {
      size_t min_size = std::min<size_t>(N1, N2);
      for (size_t i = 0; i < min_size; ++i) {
         if (lhs.value[i] < rhs.value[i]) {
               return true;
         }
         if (lhs.value[i] > rhs.value[i]) {
               return false;
         }
      }
      return N1 < N2;
   }

   /// @brief Concatenate two StingLiterals
   /// @details Removes the null terminator between the two strings
   /// @tparam N1 Size of first string
   /// @tparam N2 Size of second string
   /// @param lhs first StringLiteral to concatenate
   /// @param rhs second StringLiteral to concatenate
   /// @return Concatenated StringLiteral
   template <size_t N1, size_t N2>
   constexpr StringLiteral<N1 + N2 - 1> concat(const StringLiteral<N1>& lhs, const StringLiteral<N2>& rhs) {
      std::array<char, N1 + N2 - 1> combined = {};  // Combined array size (excluding double null terminator)

      std::copy_n(lhs.value.begin(), N1 - 1, combined.begin());
      std::copy_n(rhs.value.begin(), N2, combined.begin() + N1 - 1);

      return StringLiteral<N1 + N2 - 1>(combined);
   }

   /// @brief Concatenate a tuple of various StringLiterals
   /// @tparam Tuple tuple of StringLiterals
   /// @tparam Index Current index for recussive calls
   template <typename Tuple, std::size_t Index = 0>
   struct TupleStringConcat
   {
      static constexpr auto value() {
         if constexpr (std::tuple_size_v<Tuple> == 0)
         {
            return StringLiteral<1>("");
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
