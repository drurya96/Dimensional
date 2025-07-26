#ifndef DIMENSIONAL_STRING_LITERAL_H
#define DIMENSIONAL_STRING_LITERAL_H

#include <array> // For std::array
#include <algorithm> // For std::copy_n, std::min
#include <ostream> // For std::ostream
#include <sstream> // TODO: Is this needed..?

namespace dimension
{

   /// @brief Struct allowing strings as template parameters
   /// @details Templating on string_literal allows using raw strings as template parameters
   /// @tparam N Size of string
   template<size_t N>
   struct string_literal
   {
   public:
      static constexpr size_t size = N;
      std::array<char, N> value;

      // string_literal is meant to be used as a wrapper for strings so they may be used in template parameters, implicit conversion is acceptable.
      // cppcheck-suppress noExplicitConstructor
      constexpr string_literal(const char (&str)[N]) {
         std::copy_n(str, N, value.begin());
      }

      constexpr string_literal() : value{} {}
      // string_literal is meant to be used as a wrapper for strings so they may be used in template parameters, implicit conversion is acceptable.
      // cppcheck-suppress noExplicitConstructor
      constexpr string_literal(const std::array<char, N>& arr) : value(arr) {}
   };

   /// @brief Stream operator from a string_literal into an ostream
   /// @tparam N Size of string
   /// @param os stream to write to
   /// @param literal string_literal to write
   /// @return reference to stream written
   template<size_t N>
   std::ostream& operator<<(std::ostream& os, const string_literal<N>& literal)
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
   constexpr bool operator<(const string_literal<N1>& lhs, const string_literal<N2>& rhs) {
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

}

#endif // DIMENSIONAL_STRING_LITERAL_H
