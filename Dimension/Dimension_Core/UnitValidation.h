#ifndef DIMENSION_UNIT_VALIDATION_H
#define DIMENSION_UNIT_VALIDATION_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction

#include "Conversion.h"

namespace Dimension
{

   static constexpr bool delta = false;
   static constexpr bool quantity = true;

   // TODO: MOVE THIS
   template<typename Unit, int Num = 1, int Den = 1, bool isQuantity = delta>
   struct UnitExponent
   {
      using unit = Unit;
      using exponent = std::ratio<Num, Den>;
      static constexpr bool quantity = isQuantity;
   };








   template <typename> struct is_tuple: std::false_type {};

   template <typename ...T> struct is_tuple<std::tuple<T...>>: std::true_type {};

   template<typename T>
   concept IsBasicUnitType = requires
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
      requires is_absolute<typename T::Primary>::value;
   };

   template<typename T>
   concept IsUnitType = 
      (is_quantity_v<T> && IsBasicUnitType<typename T::unit>) || 
      IsBasicUnitType<T>;

   template<typename Tuple, std::size_t... Is>
   // cppcheck-suppress unusedFunction // Only used within concept
   constexpr bool all_satisfy_unit_constraints(std::index_sequence<Is...>)
   {
      return (IsUnitType<std::tuple_element_t<Is, Tuple>> && ...);
   }

   // The IsUnitTuple concept that ensures all types in a tuple satisfy Has_Dim
   // TODO: REMOVE
   template<typename Tuple>
   concept IsUnitTuple = 
      std::tuple_size_v<Tuple> == 0 || 
      all_satisfy_unit_constraints<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});


   template<typename T>
   concept is_unit_exponent = requires {
         typename T::unit;
         typename T::exponent;
   };

   template<typename... Ts>
   concept are_unit_exponents = (is_unit_exponent<Ts> && ...);

 
   template<typename T, typename UnitType>
   concept IsNonQuantityUnitDimension = 
      !is_quantity_v<T> &&
      requires
      {
         typename T::Dim; // Ensure T has a Dim member
      } &&
      std::is_same_v<typename T::Dim, UnitType>;

   // Helper concept for quantity temperature units
   template<typename T, typename UnitType>
   concept IsQuantityUnitDimension = 
      is_quantity_v<T> &&
      requires
      {
         typename T::unit::Dim; // Ensure T::unit has a Dim member
      } &&
      std::is_same_v<typename T::unit::Dim, UnitType>;

}

#endif // DIMENSION_UNIT_VALIDATION_H
