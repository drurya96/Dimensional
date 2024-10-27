#ifndef DIMENSION_CONCEPTS_H
#define DIMENSION_CONCEPTS_H


//#include "TypeTraits.h"

#include <concepts> // @todo may not be needed


#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction
#include <utility> // For std::declval


namespace Dimension
{
   // Forward declarations
   template<typename NumTuple, typename DenTuple>
   class BaseDimension;

   //template<typename Unit>
   struct BaseUnit;

   template<typename T>
   concept IsUnitType = requires
   {
      typename T::Dim;
      typename T::Primary;
      requires std::is_base_of_v<BaseUnit, T>;
   };


   /// @brief Check if two units are of the same dimension
   template<typename T, typename U>
   struct is_same_dim;

   /// @brief Check if two units are of the same dimension
   /// @tparam T First unit to compare
   /// @tparam U Second unit to compare
   /// @typedef value const cool indicating whether the units are the same dimension
   /// @todo When upgrading to C++20 use/replace with concept/require
   template<typename T, typename U>
   struct is_same_dim : std::integral_constant<
      bool,
      std::is_same<typename std::remove_cv<typename T::Dim>::type, typename std::remove_cv<typename U::Dim>::type>::value &&
      (T::ID == U::ID)
   > {};

   template<typename T, typename U>
   constexpr bool same_dim_v = is_same_dim<T, U>::value;

   /* UNTESTED! NEED TO VERIFY IF THIS IS NEEDED AFTER FURTHER REFACTOR!!
   template<typename T, typename U>
   concept Is_Same_Dim = 
    IsUnitType<T> && 
    IsUnitType<U> && 
    std::is_same_v<typename std::remove_cv_t<typename T::Dim>, typename std::remove_cv_t<typename U::Dim>> &&
    (T::ID == U::ID);
    */

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   template<typename Dim, typename Tuple>
   struct has_same_dim;

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   /// @tparam T The type to check for
   /// @tparam Us The types within the tuple
   /// @typedef value A constexpr bool indicating whether Us contains T
   template<typename Dim, typename... Us>
   struct has_same_dim<Dim, std::tuple<Us...>> : std::disjunction<is_same_dim<Dim, Us>...> {};

   //template<typename T, typename U>
   //concept Has_Same_Dim = has_same_dim<T, U>;


   /* UNTESTED! NEED TO VERIFY IF THIS IS NEEDED AFTER FURTHER REFACTOR!!
   template<typename Tuple, std::size_t... Is>
   constexpr bool contains_unit_of_dim(std::index_sequence<Is...>) {
      return (is_same_dim<std::tuple_element_t<Is, Tuple>> || ...);
   }

   // The IsUnitTuple concept that ensures all types in a tuple satisfy Has_Dim
   template<typename Tuple>
   concept Has_Same_Dim =
      std::tuple_size_v<Tuple> == 0 || 
      contains_unit_of_dim<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
   */






















   template<typename Tuple, std::size_t... Is>
   constexpr bool all_satisfy_unit_constraints(std::index_sequence<Is...>) {
      return (IsUnitType<std::tuple_element_t<Is, Tuple>> && ...);
   }

   // The IsUnitTuple concept that ensures all types in a tuple satisfy Has_Dim
   template<typename Tuple>
   concept IsUnitTuple =
      std::tuple_size_v<Tuple> == 0 || 
      all_satisfy_unit_constraints<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
   


} // end Dimension


#endif // DIMENSION_CONCEPTS_H
