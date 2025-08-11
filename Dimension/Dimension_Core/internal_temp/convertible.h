#ifndef DIMENSIONAL_CONVERTIBLE_H
#define DIMENSIONAL_CONVERTIBLE_H

#include <concepts>
#include <type_traits>

namespace dimension {

   template<typename From, typename To>
   struct Conversion;

   /// @brief Concept to check if Conversion is defined
   template<typename From, typename To>
   concept HasConversion = requires { Conversion<From, To>::slope; };

   /// @brief Concept to check if the unit is convertible to a primary unit (either direction)
   template<typename T>
   concept PrimaryConvertible = requires {
      requires (std::is_same_v<T, unit_primary_t<T>> || HasConversion<T, unit_primary_t<T>> || HasConversion<unit_primary_t<T>, T>);
   };

}

#endif // DIMENSIONAL_CONVERTIBLE_H