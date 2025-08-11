#ifndef DIMENSIONAL_UTILS_H
#define DIMENSIONAL_UTILS_H

#include <concepts>

#include "ratio/ratio_utils.h"
#include "symbol/symbol_utils.h"
#include "rep_type.h"
#include "units/unit_exponent.h"

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

   class base_dimension_marker;

   template<typename T>
   concept is_base_dimension = std::is_base_of_v<base_dimension_marker, T>;

   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename T, typename U>
   // TODO: Consider a requirement that T and U are dimensions
   struct is_same_dim : std::integral_constant<
      bool,
      std::is_same<typename std::remove_cv<typename T::Dim>::type, typename std::remove_cv<typename U::Dim>::type>::value &&
      (T::ID == U::ID)
   > {};


} // end dimension

#endif // DIMENSIONAL_UTILS_H
