#ifndef DIMENSION_UNIT_VALIDATION_H
#define DIMENSION_UNIT_VALIDATION_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction

#include "unit_exponent.h"
#include "unit_dim.h"
#include "../Conversion.h" // TODO: Remove this by moving PrimaryConvertible and friends to a separate header

namespace dimension
{
   template <typename> struct is_tuple: std::false_type {};

   template <typename ...T> struct is_tuple<std::tuple<T...>>: std::true_type {};

   template<typename T>
   concept IsBasicUnitType = requires
   {
      typename T::Dim; // Not sure why, but these two checks seem strictly necessary?
      typename T::Primary;
      requires std::is_same_v<unit_dim_t<T>, unit_dim_t<unit_primary_t<T>>>;
      { T::ID } -> std::convertible_to<int>;
      requires PrimaryConvertible<T>;
   };

   template<typename Tuple, std::size_t... Is>
   // cppcheck-suppress unusedFunction // Only used within concept
   constexpr bool all_satisfy_unit_constraints(std::index_sequence<Is...>)
   {
      return (IsBasicUnitType<std::tuple_element_t<Is, Tuple>> && ...);
   }

   // The IsUnitTuple concept that ensures all types in a tuple satisfy Has_Dim
   // TODO: REMOVE
   template<typename Tuple>
   concept IsUnitTuple = 
      std::tuple_size_v<Tuple> == 0 || 
      all_satisfy_unit_constraints<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});

}

#endif // DIMENSION_UNIT_VALIDATION_H
