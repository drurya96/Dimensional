#ifndef DIMENSION_GENERIC_TYPE_TRAITS_H
#define DIMENSION_GENERIC_TYPE_TRAITS_H

#include <tuple> // For std::tuple and related functions
#include <type_traits> // For std::is_same, std::remove_cv, std::disjunction, std::conditional
#include <utility> // For std::declval

namespace Dimension
{

   template<size_t N>
   struct StringLiteral {
      constexpr StringLiteral(const char (&str)[N]) {
         std::copy_n(str, N, value);
      }
      
      char value[N];
   };

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

   /// @brief Convenience alias for retrieving the type of a tuple of types
   /// @tparam Ts Parameter pack to types to concatenate
   template<typename...Ts>
   using tuple_cat_t = decltype(std::tuple_cat(std::declval<Ts>()...));

   /// @brief Struct to remove one instance of a type from a tuple of types
   template<template<typename, typename> typename Compare, typename T, typename Tuple>
   requires Comparable<Compare>
   struct RemoveOneInstance;

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Specialization for an empty tuple
   /// @typedef type An empty tuple
   template<template<typename, typename> typename Compare, typename T>
   requires Comparable<Compare>
   struct RemoveOneInstance<Compare, T, std::tuple<>>
   {
      using type = std::tuple<>;
   };

   /// @brief Struct to remove one instance of a type from a tuple of types
   /// @details Main specialization
   /// @tparam T The type to remove
   /// @tparam Head the first item of the tuple
   /// @tparam Tail the remaining items in the tuple
   /// @typedef type A tuple type matching the input tuple type, except the first instance of T is removed
   template<template<typename, typename> typename Compare, typename T, typename Head, typename... Tail>
   requires Comparable<Compare>
   struct RemoveOneInstance<Compare, T, std::tuple<Head, Tail...>>
   {
      using type = std::conditional_t < Compare<T, Head>::value,
         std::tuple<Tail...>,
         tuple_cat_t<std::tuple<Head>, typename RemoveOneInstance<Compare, T, std::tuple<Tail...>>::type>
      >;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Computes the multiset difference between two tuples, where the multiset
   ///    difference of the minuend and subtrahend contains all elements from the
   ///    minuend except those that also appear in the subtrahend.The multiplicity 
   ///    of elements is taken into account.
   template<template<typename, typename> typename Compare, typename T, typename U, typename Enable = void>
   struct tuple_diff;

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty minuend tuple
   /// @typedef type Empty tuple
   template<template<typename, typename> typename Compare>
   struct tuple_diff<Compare, std::tuple<>, std::tuple<>>
   {
      using type = std::tuple<>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty minuend tuple
   /// @typedef type Empty tuple
   template<template<typename, typename> typename Compare, typename ... subtrahendTypes>
   struct tuple_diff<Compare, std::tuple<>, std::tuple<subtrahendTypes...>>
   {
      using type = std::tuple<>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Specialization for an empty subtrahend tuple
   /// @typedef type The entire minuend tuple
   template<template<typename, typename> typename Compare, typename... restMinuendTypes>
   struct tuple_diff<Compare, std::tuple<restMinuendTypes...>, std::tuple<>>
   {
      using type = std::tuple<restMinuendTypes...>;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Diff when subtrahend is non-empty and Compare resolves to True
   ///   Removes one instance of T from subtrahendTypes
   /// @tparam Compare A comparison type-trait used to determine if T is comparable to a tuple of subtrahendTypes
   /// @tparam T The first type in the minuend tuple
   /// @tparam restMinuendTypes The remaining types in the minuend tuple
   /// @tparam subtrahendTypes The types in the subtrahend tuple
   /// @typedef type A tuple of types contain the multiset different between the minuend and subtrahend
   template<template<typename, typename> typename Compare, typename T, typename... restMinuendTypes, typename... subtrahendTypes>
   requires (
      sizeof...(subtrahendTypes) > 0 &&
      Compare<T, std::tuple<subtrahendTypes...>>::value
   )
   struct tuple_diff<Compare, std::tuple<T, restMinuendTypes...>, std::tuple<subtrahendTypes...>>
   {
      using type = typename tuple_diff
      <
         Compare, 
         std::tuple<restMinuendTypes...>, 
         typename RemoveOneInstance<is_same_dim, T, std::tuple<subtrahendTypes...>>::type
      >::type;
   };

   /// @brief Find the difference between two type-tuples
   /// @details Diff when subtrahend is non-empty and Compare resolves to False
   ///   Continues comparison with the next type in the restMinuendTypes set
   /// @tparam Compare A comparison type-trait used to determine if T is comparable to a tuple of subtrahendTypes
   /// @tparam T The first type in the minuend tuple
   /// @tparam restMinuendTypes The remaining types in the minuend tuple
   /// @tparam subtrahendTypes The types in the subtrahend tuple
   /// @typedef type A tuple of types contain the multiset different between the minuend and subtrahend
   template<template<typename, typename> typename Compare, typename T, typename... restMinuendTypes, typename... subtrahendTypes>
   requires (
      sizeof...(subtrahendTypes) > 0 &&
      !Compare<T, std::tuple<subtrahendTypes...>>::value
   )
   struct tuple_diff<Compare, std::tuple<T, restMinuendTypes...>, std::tuple<subtrahendTypes...>>
   {
      using type = tuple_cat_t<std::tuple<T>, typename tuple_diff<Compare, std::tuple<restMinuendTypes...>, std::tuple<subtrahendTypes...>>::type>;
   };


   /// @brief get the first unit in the tuple matching the dimension of T
   template<template<typename, typename> typename Compare, typename T, typename Tuple>
   struct get_first_match;

   /// @brief get the first unit in the tuple matching the dimension of T
   /// @details Specialization for no match found, return a NullUnit
   ///    This should not typical occur and is a sign of problematic code elsewhere
   /// @tparam T Unit to match against
   /// @tparam Tuple Tuple of units
   template<template<typename, typename> typename Compare, typename T>
   struct get_first_match<Compare, T, std::tuple<>> {
      using type = NullUnit;
   };

   /// @brief get the first unit in the tuple matching the dimension of T
   /// @details Primary specialization
   /// @tparam T Unit to match against
   /// @tparam Tuple Tuple of units
   /// @typedef type The type of unit of matching dimension to T
   template<template<typename, typename> typename Compare, typename T, typename Head, typename... Tail>
   struct get_first_match<Compare, T, std::tuple<Head, Tail...>> {
      using type = std::conditional_t<Compare<T, Head>::value,
         Head,
         typename get_first_match<Compare, T, std::tuple<Tail...>>::type>;
   };


} // end Dimension

#endif // DIMENSION_GENERIC_TYPE_TRAITS_H
