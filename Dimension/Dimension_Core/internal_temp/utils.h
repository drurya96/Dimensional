#ifndef DIMENSION_UTILS_H
#define DIMENSION_UTILS_H

#include <concepts>

#include "ratio_utils.h"
#include "symbol_utils.h"
#include "rep_type.h"
#include "unit_exponent.h"

namespace dimension
{

   template<typename T>
   concept is_coefficient = is_ratio<T> || is_symbol<T> || is_symbol_exponent<T>;

   template<typename T>
   concept is_coefficient_or_unit = is_unit_exponent<T> || is_coefficient<T>;

   // Forward declarations
   template<rep_type Rep, is_coefficient_or_unit... Ts>
   class base_dimension_impl;

   template<typename T, typename Rep, typename... Exponents>
   concept dimension_convertible_to = std::is_convertible_v<
      T,
      base_dimension_impl<Rep, Exponents...>
   >;

   template<typename... Units>
   struct FlipExponents;
   
   template<>
   struct FlipExponents<std::tuple<>> {
       using units = std::tuple<>;
   };
   
   template<typename Unit, typename... Rest>
   struct FlipExponents<std::tuple<Unit, Rest...>> {
       using units = tuple_cat_t<
           std::tuple<unit_exponent<typename Unit::unit, -Unit::exponent::num, Unit::exponent::den>>,
           typename FlipExponents<std::tuple<Rest...>>::units
       >;
   };

} // end dimension

#endif // DIMENSION_UTILS_H
