#ifndef DIMENSION_CONCEPTS_H
#define DIMENSION_CONCEPTS_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction
#include <utility> // For std::declval

namespace Dimension
{
   // Forward declarations
   template<typename Unit>
   struct BaseUnit;

   /// @brief A type-trait with void Dim and Primary, only used to satisfy a metaprogramming condition
   struct NullUnit
   {
      using Dim = void;
      using Primary = void;
      static constexpr int ID = 0;
   };

   template<template<typename, typename> typename Compare>
   concept Comparable = requires
   {
      { Compare<NullUnit, NullUnit>::value } -> std::convertible_to<bool>;
   };

   template<typename T>
   concept IsUnitType = requires
   {
      typename T::Dim;
      typename T::Primary;
      requires std::is_base_of_v<BaseUnit<T>, T>;
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

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   template<typename Dim, typename Tuple>
   struct has_same_dim;

   /// @brief Struct to check if a tuple of units contains a unit of the given Dimension
   /// @tparam T The type to check for
   /// @tparam Us The types within the tuple
   /// @typedef value A constexpr bool indicating whether Us contains T
   template<typename Dim, typename... Us>
   struct has_same_dim<Dim, std::tuple<Us...>> : std::disjunction<is_same_dim<Dim, Us>...> {};

   template<typename Tuple, std::size_t... Is>
   constexpr bool all_satisfy_unit_constraints(std::index_sequence<Is...>) {
      return (IsUnitType<std::tuple_element_t<Is, Tuple>> && ...);
   }

   // The IsUnitTuple concept that ensures all types in a tuple satisfy Has_Dim
   template<typename Tuple>
   concept IsUnitTuple =
      std::tuple_size_v<Tuple> == 0 || 
      all_satisfy_unit_constraints<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});

   /// Concept to check if T has a numeric slope attribute.
   template<typename T>
   concept HasSlope = requires { T::slope; } && std::is_arithmetic_v<decltype(T::slope)>;

   /// Concept to check if T has a numeric offset attribute.
   template<typename T>
   concept HasOffset = requires { T::offset; } && std::is_arithmetic_v<decltype(T::offset)>;

   /// @brief Return the slope as a constexpr if one exists,
   ///    otherwise return 1.0
   template<typename T>
   constexpr PrecisionType GetSlope()
   {
      if constexpr (HasSlope<T>)
      {
         return T::slope;
      }
      else
      {
         return 1.0;
      }
   }

   /// @brief Return the offset as a constexpr if one exists,
   ///    otherwise return 0.0
   template<typename T>
   constexpr PrecisionType GetOffset()
   {
      
      if constexpr (HasOffset<T>)
      {
         return T::offset;
      }
      else
      {
         return 0.0;
      }
   }

} // end Dimension

#endif // DIMENSION_CONCEPTS_H
