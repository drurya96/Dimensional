#ifndef DIMENSIONAL_CONVERTIBLE_H
#define DIMENSIONAL_CONVERTIBLE_H

#include <concepts>
#include <type_traits>

#include "new_unit_stuff.h"

namespace dimension {

   template<typename From, typename To>
   struct Conversion;

   /// @brief Concept to check if Conversion is defined
   template<typename From, typename To>
   concept HasConversion = requires { typename Conversion<From, To>::scale; };

   /// @brief Concept to check if the unit is convertible to a primary unit (either direction)
   template<typename T>
   concept PrimaryConvertible = requires {
      requires (std::is_same_v<T, unit_primary_t<T>> || HasConversion<T, unit_primary_t<T>> || HasConversion<unit_primary_t<T>, T>);
   };

}

#endif // DIMENSIONAL_CONVERTIBLE_H