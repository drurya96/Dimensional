#ifndef DIMENSION_UNIT_VALIDATION_H
#define DIMENSION_UNIT_VALIDATION_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction

#include "Conversion.h"

namespace Dimension
{
   template <typename> struct is_tuple: std::false_type {};

   template <typename ...T> struct is_tuple<std::tuple<T...>>: std::true_type {};

   template<typename T>
   concept IsUnitType = requires
   {
      typename T::Dim;
      typename T::Primary;
      requires std::is_same_v<typename T::Dim, typename T::Primary::Dim>;
      typename T::NumTuple;
      typename T::DenTuple;
      requires is_tuple<typename T::NumTuple>::value;
      requires is_tuple<typename T::DenTuple>::value;
      { T::ID } -> std::convertible_to<int>;
      requires PrimaryConvertible<T>;
   };

   template<typename Tuple, std::size_t... Is>
   constexpr bool all_satisfy_unit_constraints(std::index_sequence<Is...>) {
      return (IsUnitType<std::tuple_element_t<Is, Tuple>> && ...);
   }

   // The IsUnitTuple concept that ensures all types in a tuple satisfy Has_Dim
   template<typename Tuple>
   concept IsUnitTuple =
      std::tuple_size_v<Tuple> == 0 || 
      all_satisfy_unit_constraints<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});

}

#endif // DIMENSION_UNIT_VALIDATION_H
